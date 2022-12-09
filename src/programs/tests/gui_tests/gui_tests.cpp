// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>

TEST_MAIN()

#include <core/memory.hpp>
#include <core/math/color.hpp>
#include <core/math/vec3.hpp>
#include <core/math/mat4.hpp>

#include <gpu/context.hpp>
#include <gpu/buffer.hpp>
#include <gpu/graphics_command_list.hpp>
#include <gpu/graphics_pipeline.hpp>

using namespace gpu;

#include <dxc/dxc.hpp>

#include <gui/context.hpp>
#include <gui/window.hpp>

#include <draw/canvas.hpp>
#include <draw/shapes/rect.hpp>

using namespace draw;

static const char* source = R"#(
cbuffer bufs : register(b0) {
	float4x4 local_to_projection;
}

struct VSInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};


struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float4 scissor : SCISSOR;
	float2 uv : UV;
	uint texture : TEXTURE;
};

PSInput vs_main(VSInput input)
{
	PSInput result;

	float4 adjusted = float4(input.position.x, input.position.y, input.position.z, 1.0);
	result.position = mul(local_to_projection, adjusted);
	result.color = input.color;

	return result;
}

float4 ps_main(PSInput input) : SV_TARGET
{
	return input.color;
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
	};
	pipeline_config.color_attachments.push(Format::RGBA_U8);
	auto pipeline = GraphicsPipeline::make(core::move(pipeline_config));

	f32 x = 0.f;

	bool first_show = false;
	while (true) {
		gui::pump_events();

		x += 0.1f;

		auto canvas = Canvas::make();
		{
			const auto bounds = AABB2f32::from_center(
				{ core::sin(x) * 100.f, core::cos(x) * 100.f },
				200.f
			);
			canvas.paint(Rect{ bounds });
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
			auto& back_buffer = window->swapchain().back_buffer();
			const auto back_buffer_size = back_buffer.size();

			const auto projection = Mat4f32::orthographic(
				(f32)back_buffer_size.width,
				(f32)back_buffer_size.height,
				0.1f,
				1000.f
			);

			gcr.texture_barrier(
				back_buffer,
				Layout::Present,
				Layout::ColorAttachment
			).render_pass(back_buffer, nullptr, [&](RenderPassRecorder& rpr) {
				rpr
					.push_constants(&projection)
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
