// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>

TEST_MAIN()

#include <core/memory.hpp>
#include <core/math/color.hpp>
#include <core/math/mat4.hpp>
#include <core/math/quat.hpp>
#include <core/math/vec3.hpp>

#include <gpu/context.hpp>
#include <gpu/buffer.hpp>
#include <gpu/graphics_command_list.hpp>
#include <gpu/graphics_pipeline.hpp>

using namespace gpu;

#include <dxc/dxc.hpp>

#include <gui/context.hpp>
#include <gui/window.hpp>

#include <draw/canvas.hpp>
#include <draw/font.hpp>
#include <draw/shapes/rect.hpp>
#include <draw/shapes/text.hpp>

using namespace draw;

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

TEST_CASE("guis can create windows") {
	gpu::init();
	gui::init();

	// Create a window that is invisible
	gui::WindowConfig config = {
		.title = "Test",
		.visible = false,
	};
	auto window = gui::Window::make(config);

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

	//auto cwd = core::cwd();

	auto font = Font::import("../../res/consola.ttf").unwrap();

	f32 x = 0.f;

	bool first_show = false;
	while (true) {
		gui::pump_events();

		x += 0.1f;

		auto& back_buffer = window->swapchain().back_buffer();
		const auto back_buffer_size = back_buffer.size().cast<f32>();

		auto canvas = Canvas::make();
		{
			const auto bounds = AABB2f32::from_center(
				{ core::sin(x) * 100.f, core::cos(x) * 100.f },
				200.f
			);
			canvas.paint(Rect{ bounds });
		}
		{
			const Vec2f32 min = { 0, back_buffer_size.height - font.new_line()};
			const Vec2f32 max = Vec2f32(back_buffer_size.width, back_buffer_size.height);

			const auto bounds = AABB2f32::from_min_max(min, max);
			canvas.paint(Text("Hello World", bounds, font, (core::sin(x * 0.1f) + 1.f) * 20.f + 64.f));
			//canvas.paint(Rect{ bounds });
		}
		const auto vertices = canvas.vertices();

		// Create the triangle vertex buffer
		auto vertex_buffer = Buffer::make(
			BU_Vertex,
			BufferKind::Upload,
			vertices.len(),
			sizeof(Vertex)
		);
		vertex_buffer.map([&](Slice<u8> slice) {
			core::mem::copy(slice.begin(), vertices.cbegin(), slice.len());
		});

		auto command_list = GraphicsCommandList::make();
		command_list.record([&](GraphicsCommandRecorder& gcr) {
			const auto projection = Mat4f32::orthographic(
				back_buffer_size.width,
				back_buffer_size.height,
				0.1f,
				1000.f
			);

			const auto view = Mat4f32::transform(
				{ -back_buffer_size.width / 2.f, -back_buffer_size.height / 2.f, 0 },
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
					.set_pipeline(pipeline)
					.clear_color(LinearColor::black)
					.set_vertices(vertex_buffer)
					.draw(vertices.len(), 0);
			}).texture_barrier(
				back_buffer,
				Layout::ColorAttachment,
				Layout::Present
			);
		});
		command_list.submit();

		window->swapchain().present();

		// Show the window after we've drawn to the window once
		if (!first_show) {
			window->set_visible(true);
			first_show = true;
		}
	}
}
