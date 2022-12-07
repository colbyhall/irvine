// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>

TEST_MAIN()

#include <core/memory.hpp>
#include <core/math/vec3.hpp>
#include <core/math/color.hpp>

#include <gpu/context.hpp>
#include <gpu/buffer.hpp>
#include <gpu/graphics_command_list.hpp>
#include <gpu/graphics_pipeline.hpp>

#include <dxc/dxc.hpp>

#include <gui/context.hpp>
#include <gui/window.hpp>

static const char* source = R"#(
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput vs_main(float3 position : POSITION, float4 color: COLOR)
{
	PSInput result;

	float4 adjusted = float4(position.x, position.y, position.z, 1.0);
	result.position = adjusted;
	result.color = color;

	return result;
}

float4 ps_main(PSInput input) : SV_TARGET
{
	return input.color;
}
)#";

using namespace gpu;

struct Vertex {
	Vec3f32 position;
	LinearColor color;
};

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

	// Create the triangle vertex buffer
	auto triangle_vertices = Buffer::make(
		BU_Vertex,
		BufferKind::Upload,
		3,
		sizeof(Vertex)
	);
	triangle_vertices.map([](Slice<u8> slice) {
		Vertex vertices[3] = {
			Vertex {
				{ -0.5f, -0.5f, 0 },
				LinearColor::red
			},
			Vertex {
				{ 0, 0.5f, 0 },
				LinearColor::green
			},
			Vertex {
				{ 0.5f, -0.5f, 0 },
				LinearColor::blue
			}
		};
		core::mem::copy(&slice[0], vertices, slice.len());
	});

	bool first_show = false;
	while (true) {
		gui::pump_events();

		auto command_list = GraphicsCommandList::make();
		command_list.record([&](GraphicsCommandRecorder& gcr) {
			auto& back_buffer = window->swapchain().back_buffer();
			gcr.texture_barrier(
				back_buffer,
				Layout::Present,
				Layout::ColorAttachment
			).render_pass(back_buffer, nullptr, [&](RenderPassRecorder& rpr) {
				rpr
					.set_pipeline(pipeline)
					.clear_color(LinearColor::black)
					.set_vertices(triangle_vertices)
					.draw(3, 0);
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
