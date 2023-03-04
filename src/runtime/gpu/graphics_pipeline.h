// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gpu/shader.h>
#include <gpu/texture.h>

GPU_NAMESPACE_BEGIN

enum class DrawMode : u8 {
	Fill,
	Line,
	Point
};

enum class CullMode : u8 {
	None,
	Front,
	Back
};

enum class CompareOp : u8 {
	/// `false`
	Never,
	/// `A < B`
	Less,
	/// `A == B`
	Equal,
	/// `A < B || A == B`
	LessOrEqual,
	/// `A > B`
	Greater,
	/// `A != B`
	NotEqual,
	/// `A > B || A == B`
	GreaterOrEqual,
	/// `true`
	Always,
};

enum class BlendOp : u8 {
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max,
};

enum class BlendFactor : u8 {
	Zero,
	One,

	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,

	SrcAlpha,
	OneMinusSrcAlpha,
};

struct GraphicsPipelineConfig {
	VertexShader vertex_shader;
	PixelShader pixel_shader;

	Array<Format> color_attachments;
	Format depth_attachment = Format::Undefined;

	DrawMode draw_mode = DrawMode::Fill;
	f32 line_width = 1.f;
	CullMode cull_mode = CullMode::None;

	bool blend_enabled = false;

	BlendFactor src_color_blend_factor = BlendFactor::One;
	BlendFactor dst_color_blend_factor = BlendFactor::One;
	BlendOp color_blend_op = BlendOp::Add;

	BlendFactor src_alpha_blend_factor = BlendFactor::One;
	BlendFactor dst_alpha_blend_factor = BlendFactor::One;
	BlendOp alpha_blend_op = BlendOp::Add;

	bool depth_test = false;
	bool depth_write = false;
	CompareOp depth_compare = CompareOp::Always;
};

class GraphicsPipelineInterface : private NonCopyable {
public:
	virtual const GraphicsPipelineConfig& config() const = 0;
	virtual ~GraphicsPipelineInterface() {}
};

class GraphicsPipeline {
public:
	static GraphicsPipeline make(GraphicsPipelineConfig&& config);

	inline const GraphicsPipelineConfig& config() const { return m_interface->config(); }

	template <typename T = GraphicsPipelineInterface>
	inline T const& interface() const {
		static_assert(core::is_base_of<GraphicsPipelineInterface, T>, "T is not derived of GraphicsPipelineInterface");
		return static_cast<const T&>(*m_interface);
	}

private:
	GraphicsPipeline(Shared<GraphicsPipelineInterface, SMode::Atomic>&& interface) : m_interface(core::move(interface)) { }

	Shared<GraphicsPipelineInterface, SMode::Atomic> m_interface;
};

GPU_NAMESPACE_END
