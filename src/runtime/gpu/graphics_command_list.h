// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/shared.h>
#include <core/containers/function.h>
#include <core/math/aabb2.h>
#include <core/math/color.h>

#include <gpu/gpu.h>

GPU_NAMESPACE_BEGIN

class Buffer;
class Texture;
class GraphicsPipeline;
class GraphicsCommandList;

enum class Layout : u8 {
	Undefined,
	General,
	ColorAttachment,
	DepthAttachment,
	TransferSrc,
	TransferDst,
	ShaderReadOnly,
	Present,
};

class GraphicsCommandListInterface : private NonCopyable {
public:
	virtual void begin_recording() = 0;

	virtual void copy_buffer_to_texture(const Texture& dst, const Buffer& src) = 0;
	virtual void copy_buffer_to_buffer(const Buffer& dst, const Buffer& src) = 0;

	virtual void texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) = 0;

	virtual void begin_render_pass(const Texture& color, Option<Texture const&> depth) = 0;

	virtual void set_scissor(Option<Aabb2f32> scissor) = 0;
	virtual void clear_color(const LinearColor& color) = 0;
	virtual void clear_depth_stencil(f32 depth, u8 stencil) = 0;
	virtual void set_pipeline(const GraphicsPipeline& pipeline) = 0;
	virtual void set_vertices(const Buffer& buffer) = 0;
	virtual void set_indices(const Buffer& buffer) = 0;
	virtual void push_constant(const void* ptr) = 0;
	// fn bind_constants(&mut self, name: &str, buffer: &Arc<backend::Buffer>, index: usize);
	// fn bind_texture(&mut self, name: &str, texture: &Arc<backend::Texture>);
	virtual void draw(usize vertex_count, usize first_vertex) = 0;
	virtual void draw_indexed(usize index_count, usize first_index) = 0;

	virtual void end_render_pass() = 0;

	virtual void end_recording() = 0;
	virtual void submit(const GraphicsCommandList& command_list) = 0;
	// fn submit(self, wait_on: &[Receipt]) -> Result<Arc<backend::Receipt>>;

	virtual ~GraphicsCommandListInterface() {}
};

class GraphicsCommandRecorder;
class RenderPassRecorder;

class GraphicsCommandList {
public:
	static GraphicsCommandList record(FunctionRef<void(GraphicsCommandRecorder&)> callable);
	void submit();
	inline GraphicsCommandList clone() const { return m_interface.clone(); }

private:
	GraphicsCommandList(Shared<GraphicsCommandListInterface, SMode::Atomic>&& interface)
		: m_interface(core::move(interface)) {}

	Shared<GraphicsCommandListInterface, SMode::Atomic> m_interface;
};

class GraphicsCommandRecorder {
public:
	GraphicsCommandRecorder& copy_buffer_to_texture(const Texture& dst, const Buffer& src);
	GraphicsCommandRecorder& copy_buffer_to_buffer(const Buffer& dst, const Buffer& src);
	GraphicsCommandRecorder& texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout);

	GraphicsCommandRecorder& render_pass(
		const Texture& color,
		Option<const Texture&> depth,
		FunctionRef<void(RenderPassRecorder&)> callable
	);

private:
	inline GraphicsCommandRecorder(GraphicsCommandListInterface& interface) : m_interface(interface) {}
	friend class GraphicsCommandList;

	GraphicsCommandListInterface& m_interface;
};

class RenderPassRecorder {
public:
	RenderPassRecorder& clear_color(LinearColor color);
	RenderPassRecorder& clear_depth_stencil(f32 depth, u8 stencil);
	RenderPassRecorder& set_pipeline(const GraphicsPipeline& pipeline);
	RenderPassRecorder& set_vertices(const Buffer& buffer);
	RenderPassRecorder& set_indices(const Buffer& buffer);
	RenderPassRecorder& push_constants(const void* ptr);
	RenderPassRecorder& draw(usize vertex_count, usize first_vertex = 0);
	RenderPassRecorder& draw_index(usize index_count, usize first_index = 0);

private:
	inline RenderPassRecorder(GraphicsCommandListInterface& interface) : m_interface(interface) {}
	friend class GraphicsCommandRecorder;

	GraphicsCommandListInterface& m_interface;
};

GPU_NAMESPACE_END
