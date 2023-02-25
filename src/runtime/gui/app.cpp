// Copyright Colby Hall. All Rights Reserved.

#include <core/math/aabb2.h>
#include <core/math/mat4.h>
#include <core/containers/string_view.h>

#include <gui/app.h>
#include <gui/builder.h>
#include <gui/draw.h>

#include <draw/canvas.h>
#include <draw/shapes/rect.h>
#include <draw/shapes/text.h>

#include <gpu/buffer.h>
#include <gpu/graphics_command_list.h>
#include <gpu/graphics_pipeline.h>
#include <gpu/swapchain.h>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
	#include <core/containers/wstring.h>
	#include <core/library.h>
#endif // #ifdef PLATFORM_WINDOWS

#include <dxc/dxc.h>

using namespace gpu;
using namespace draw;

#ifdef PLATFORM_WINDOWS
	typedef enum PROCESS_DPI_AWARENESS {
		PROCESS_DPI_UNAWARE,
		PROCESS_SYSTEM_DPI_AWARE,
		PROCESS_PER_MONITOR_DPI_AWARE
	} PROCESS_DPI_AWARENESS;

	enum MONITOR_DPI_TYPE {
		MDT_EFFECTIVE_DPI,
		MDT_ANGULAR_DPI,
		MDT_RAW_DPI,
		MDT_DEFAULT
	};

	typedef HRESULT(*SetProcessDPIAwareness)(PROCESS_DPI_AWARENESS value);
	typedef HRESULT(*GetDPIForMonitor)(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY);
#endif // #ifdef PLATFORM_WINDOWS

GUI_NAMESPACE_BEGIN

static const char* source = R"#(
cbuffer bufs : register(b0) {
	float4x4 local_to_projection;
}

Texture2D texture2d_table[] : register(t0);
SamplerState sampler_table : register(s0);

struct PSInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float4 scissor : SCISSOR;
	float2 uv : UV;
	float2 screen_pos : SCREEN_POS;
	uint tex2d : TEX;
};

struct VSInput {
	float3 position : POSITION;
	float4 color : COLOR;
	float4 scissor : SCISSOR;
	float2 uv : UV;
	uint tex2d : TEX;
};

PSInput vs_main(VSInput input) {
	PSInput result;

	result.position = mul(local_to_projection, float4(input.position, 1.0));
	result.color = input.color;
	result.scissor = input.scissor;
	result.uv = input.uv;
	result.screen_pos = input.position.xy;
	result.tex2d = input.tex2d;

	return result;
}

float4 ps_main(PSInput input) : SV_TARGET {
	bool in_scissor =
		input.screen_pos.x >= input.scissor.x &&
		input.screen_pos.y >= input.scissor.y &&
		input.screen_pos.x <= input.scissor.z &&
		input.screen_pos.y <= input.scissor.w;

	float4 output = input.color;
	if (true) {
		if (input.tex2d == 0) {
			return input.color;
		}
		float dist_alpha_mask = texture2d_table[input.tex2d].Sample(sampler_table, input.uv, 0).x;
		output.w *= smoothstep(0.65, 0.7, dist_alpha_mask);
	}
	return output;
}
)#";

App App::make() {
#ifdef PLATFORM_WINDOWS
	static auto shcore = Library::open("shcore.dll");
	if (shcore) {
		auto& actual = shcore.as_mut().unwrap();
		auto SetProcessDpiAwareness = (SetProcessDPIAwareness)actual.find("SetProcessDpiAwareness");
		if (SetProcessDpiAwareness) SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	}
#endif // #ifdef PLATFORM_WINDOWS

	auto font = Font::import("../../res/consola.ttf").unwrap();

	// Compile the vertex shader using source
	dxc::Input vertex_input = {
		source,
		"vs_main",
		ShaderType::Vertex
	};
	auto vertex_output = dxc::compile(vertex_input).unwrap();
	auto vertex_shader = VertexShader::make(
		core::move(vertex_output.binary),
		core::move(vertex_output.input_parameters)
	);

	// Compile the pixel shader using source
	dxc::Input pixel_input = {
		source,
		"ps_main",
		ShaderType::Pixel
	};
	auto pixel_output = dxc::compile(pixel_input).unwrap();
	auto pixel_shader = PixelShader::make(core::move(pixel_output.binary));

	// Create the graphics pipeline
	GraphicsPipelineConfig pipeline_config = {
		.vertex_shader = core::move(vertex_shader),
		.pixel_shader = core::move(pixel_shader),
		.blend_enabled = true,
		.src_color_blend_factor = BlendFactor::SrcAlpha,
		.dst_color_blend_factor = BlendFactor::OneMinusSrcAlpha,
	};
	pipeline_config.color_attachments.push(Format::RGBA_U8);
	auto pipeline = GraphicsPipeline::make(core::move(pipeline_config));

	return App(core::move(pipeline), core::move(font));
}

static App* g_app = nullptr;

void App::run(FunctionRef<void(AppBuilder&)> frame) {
	g_app = this;

	for (;;) {
#ifdef PLATFORM_WINDOWS
		MSG msg;
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
#endif // #ifdef PLATFORM_WINDOWS

		AppBuilder builder(*this);
		frame(builder);
	}
}

struct WindowMemory : public Memory {
	void* handle;
	Swapchain swapchain;
	Aabb2f32 bounds;
	f32 dpi;

	WindowMemory(void* in_handle, Swapchain&& in_swapchain, Aabb2f32 in_bounds, f32 in_dpi) :
		handle(in_handle),
		swapchain(core::move(in_swapchain)),
		bounds(in_bounds),
		dpi(in_dpi) {}
};

#ifdef PLATFORM_WINDOWS

// NOTE: Could #include <hidusage.h> for these defines
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

static Option<Aabb2f32> get_screen_space_bounds(HWND hWnd) {
	const auto dpi = (f32)GetDpiForWindow(hWnd) / 96.f;
	RECT rect;
	if (GetClientRect(hWnd, &rect)) {
		POINT MIN = { rect.left, rect.bottom };
		POINT MAX = { rect.right, rect.top };
		ClientToScreen(hWnd, &MIN);
		ClientToScreen(hWnd, &MAX);

		const int monitor_height = GetSystemMetrics(SM_CYSCREEN);
		const auto min = Vec2f32{ (f32)MIN.x, (f32)(monitor_height - MIN.y) } / dpi;
		const auto max = Vec2f32{ (f32)MAX.x, (f32)(monitor_height - MAX.y) } / dpi;
		return Aabb2f32::from_min_max(min, max);
	}
	return nullptr;
}

static LRESULT CALLBACK window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	auto id = Id::from_raw((u64)GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	auto opt_window = g_app->memory().get_mut<WindowMemory>(id);
	if (!opt_window) {
		return DefWindowProcW(hWnd, Msg, wParam, lParam);
	}
	auto& window = opt_window.unwrap();

	switch (Msg) {
	case WM_DESTROY:
	case WM_CLOSE:
		ExitProcess(0);
		break;
	case WM_MOVE: {
		auto opt_bounds = get_screen_space_bounds(hWnd);
		if (opt_bounds) {
			window.bounds = opt_bounds.unwrap();
		}
		return 0;
	}
	case WM_SIZE: {
		auto opt_bounds = get_screen_space_bounds(hWnd);
		if (opt_bounds) {
			window.bounds = opt_bounds.unwrap();
		}
		window.swapchain.resize();
		return 0;
	}
	default:
		return DefWindowProcW(hWnd, Msg, wParam, lParam);
	}
}


void AppBuilder::window(const WindowConfig& config, FunctionRef<void(Builder&)> contents) {
	const auto id = Id(config.title);
	auto& memory = m_app.m_memory.get_mut_or_else<WindowMemory>(id, [&]() {
		HINSTANCE hInstance = GetModuleHandleA(nullptr);

		DWORD dwStyle = WS_OVERLAPPEDWINDOW;

		WNDCLASSEXW window_class = {};
		window_class.cbSize = sizeof(WNDCLASSEXW);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = &window_proc;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = hInstance;
		window_class.hIcon = ::LoadIconW(hInstance, nullptr);
		window_class.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
		window_class.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
		window_class.lpszMenuName = nullptr;
		window_class.lpszClassName = L"window_class";
		window_class.hIconSm = ::LoadIconW(hInstance, nullptr);

		static bool registered = true;
		if (registered) {
			registered = false;
			const ATOM atom = RegisterClassExW(&window_class);
			ASSERT(atom != 0);
		}

		const auto dpi = (f32)GetDpiForSystem() / 96.f;
		const auto size = Vec2u32{ 1280, 720 };

		RECT adjusted_rect = {
			.left = 0,
			.top = 0,
			.right = (LONG)((f32)size.width * dpi),
			.bottom = (LONG)((f32)size.height * dpi)
		};
		AdjustWindowRect(&adjusted_rect, dwStyle, 0);

		const int width = adjusted_rect.right - adjusted_rect.left;
		const int height = adjusted_rect.bottom - adjusted_rect.top;

		const int monitor_width = GetSystemMetrics(SM_CXSCREEN);
		const int monitor_height = GetSystemMetrics(SM_CYSCREEN);

		const int x = monitor_width / 2 - width / 2;
		const int y = monitor_height / 2 - height / 2;

		WString title;
		title.push(config.title);

		HWND handle = CreateWindowExW(
			0,
			window_class.lpszClassName,
			title.ptr(),
			dwStyle,
			x, y, width, height,
			nullptr, nullptr,
			hInstance,
			nullptr
		);
		ASSERT(handle != INVALID_HANDLE_VALUE);

		RAWINPUTDEVICE rid;
		rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		rid.usUsage = HID_USAGE_GENERIC_MOUSE;
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.hwndTarget = handle;
		const auto rid_success = RegisterRawInputDevices(&rid, 1, sizeof(rid));
		ASSERT(rid_success);

		int show = SW_HIDE;
		if (config.visible) {
			if (config.maximized) {
				show = SW_SHOWMAXIMIZED;
			}
			else if (config.minimized) {
				show = SW_SHOWMINIMIZED;
			}
			else {
				show = SW_SHOWDEFAULT;
			}
		}
		ShowWindow(handle, show);

		auto swapchain = Swapchain::make(handle);
		SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR)(u64)id);

		// Windows screen coordinates originate in top left corner where +Y is down
		// ours is bottom left corner where +Y is up
		const auto min = Vec2f32 { (f32)x, (f32)(monitor_height - (y + height)) } / dpi;
		const auto max = min + size.cast<f32>();

		return WindowMemory(
			handle,
			core::move(swapchain),
			Aabb2f32::from_min_max(min, max),
			dpi
		);
	});

	// Each native window contains their own canvas
	auto canvas = gui::Canvas::make();
	auto builder = Builder(m_app, canvas, memory.bounds);
	contents(builder);

	// Convert canvas to vertex buffer
	auto draw_canvas = draw::Canvas::make();
	for (const gui::Shape& shape : canvas.shapes()) {
		draw_canvas.set_color(shape.color);
		switch (shape.type) {
			case ShapeType::Rect: {
				auto rect = draw::Rect(shape.bounds);
				rect.round(shape.rect.round);
				if (shape.rect.stroke) {
					rect.stroke(shape.rect.stroke.unwrap());
				}
				draw_canvas.paint(rect);
			} break;
			case ShapeType::Text: {
				draw_canvas.paint(draw::Text(
					shape.text.text,
					shape.bounds,
					m_app.m_font,
					shape.text.size
				));
			} break;
		}
	}
	const auto vertices = draw_canvas.vertices();
	const auto indices = draw_canvas.indices();

	auto& back_buffer = memory.swapchain.back_buffer();
	const auto bounds = memory.bounds;

	// Draw the gui on the gpu
	if (!vertices.is_empty()) {
		// Upload the vertices and indices to the gpu
		auto vertex_buffer = Buffer::make(
			BU_Vertex,
			BufferKind::Upload,
			vertices.len(),
			sizeof(Vertex)
		);
		vertex_buffer.map([&](Slice<u8> slice) {
			core::copy(slice.begin(), vertices.cbegin(), slice.len());
		});

		auto index_buffer = Buffer::make(
			BU_Index,
			BufferKind::Upload,
			indices.len(),
			sizeof(u32)
		);
		index_buffer.map([&](Slice<u8> slice) {
			core::copy(slice.begin(), indices.cbegin(), slice.len());
		});

		GraphicsCommandList::record([&](GraphicsCommandRecorder& gcr) {
			const auto bounds_size = bounds.size();
			const auto projection = Mat4f32::orthographic(
				bounds_size.width,
				bounds_size.height,
				0.1f,
				1000.f
			);

			const auto view = Mat4f32::transform(
				{ -bounds.position().x, -bounds.position().y, 0 },
				Quatf32::identity,
				1
			);

			const auto local_to_projection = projection * view;

			gcr.texture_barrier(
				back_buffer,
				Layout::Present,
				Layout::ColorAttachment
			).render_pass(back_buffer, nullptr, [&](RenderPassRecorder& rpr) {
				rpr
					.push_constants(&local_to_projection)
					.set_pipeline(m_app.m_pipeline)
					.clear_color(LinearColor::black)
					.set_vertices(vertex_buffer)
					.set_indices(index_buffer)
					.draw_index(indices.len(), 0);
			}).texture_barrier(
				back_buffer,
				Layout::ColorAttachment,
				Layout::Present
			);
		}).submit();
	} else {
		GraphicsCommandList::record([&](GraphicsCommandRecorder& gcr) {
			gcr.texture_barrier(
				back_buffer,
				Layout::Present,
				Layout::ColorAttachment
			).render_pass(back_buffer, nullptr, [&](RenderPassRecorder& rpr) {
				rpr.clear_color(LinearColor::black);
			}).texture_barrier(
				back_buffer,
				Layout::ColorAttachment,
				Layout::Present
			);
		}).submit();
	}
	memory.swapchain.present();
}

#endif // #ifdef PLATFORM_WINDOWS

GUI_NAMESPACE_END
