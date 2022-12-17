// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.h>
#include <gpu/buffer.h>
#include <gpu/graphics_command_list.h>
#include <gpu/graphics_pipeline.h>

using namespace gpu;

#include <dxc/dxc.h>

#include <gui/context.h>
#include <gui/window.h>

#include <game/ecs/world.h>
#include <game/transform.h>

using namespace game;

#include <core/platform/windows.h>

static const char* gui_shader = R"#(
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

static const char* game_shader = R"#(
cbuffer bufs : register(b0) {
	float4x4 local_to_projection;
}

Texture2D texture2d_table[] : register(t0);
SamplerState sampler_table : register(s0);

struct PSInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct VSInput {
	float3 position : POSITION;
	float4 color : COLOR;
};

PSInput vs_main(VSInput input) {
	PSInput result;

	result.position = mul(local_to_projection, float4(input.position, 1.0));
	result.color = input.color;

	return result;
}

float4 ps_main(PSInput input) : SV_TARGET {
	return input.color;
}
)#";

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
) {
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;

	gpu::init();
	gui::init();

#if 0
	// Create the gui graphics pipeline
	Option<GraphicsPipeline> opt_gui_pipeline = nullptr;;
	{
		// Compile the vertex shader using source
		dxc::Input vertex_input = {
			gui_shader,
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
			gui_shader,
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
		opt_gui_pipeline = GraphicsPipeline::make(core::move(pipeline_config));
	}
	auto gui_pipeline = opt_gui_pipeline.unwrap();
#endif

	// Create the game graphics pipeline
	Option<GraphicsPipeline> opt_game_pipeline = nullptr;;
	{
		// Compile the vertex shader using source
		dxc::Input vertex_input = {
			game_shader,
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
			game_shader,
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
		opt_game_pipeline = GraphicsPipeline::make(core::move(pipeline_config));
	}
	auto game_pipeline = opt_game_pipeline.unwrap();

	gui::WindowConfig config = {
		.title = "Test",
	};
	auto window = gui::Window::make(config);

	auto world = World();

	while(true) {
		gui::pump_events();
	}
}
