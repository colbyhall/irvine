// Copyright Colby Hall. All Rights Reserved.

#include <gpu/buffer.hpp>
#include <gpu/context.hpp>
#include <gpu/d3d12/d3d12_graphics_command_list.hpp>

GPU_NAMESPACE_BEGIN

GraphicsCommandList GraphicsCommandList::make() {
	auto& context = Context::the();

	Option<Unique<GraphicsCommandListInterface>> interface = nullptr;
	switch (context.backend()) {
		case Backend::D3D12:
			interface = make_unique<D3D12GraphicsCommandList>();
			break;
	}

	return GraphicsCommandList { interface.unwrap() };
}

void GraphicsCommandList::record(FunctionRef<void(GraphicsCommandRecorder&)> callable) {
	GraphicsCommandListInterface& interface = *m_interface;
	interface.begin_recording();
	GraphicsCommandRecorder recorder(interface);
	callable(recorder);
	interface.end_recording();
}

void GraphicsCommandList::submit() {
	GraphicsCommandListInterface& interface = *m_interface;
	interface.submit();
}

GraphicsCommandRecorder& GraphicsCommandRecorder::copy_buffer_to_texture(const Texture& dst, const Buffer& src) {
	m_interface.copy_buffer_to_texture(dst, src);
	return *this;
}

GraphicsCommandRecorder& GraphicsCommandRecorder::copy_buffer_to_buffer(const Buffer& dst, const Buffer& src) {
	m_interface.copy_buffer_to_buffer(dst, src);
	return *this;
}

GraphicsCommandRecorder& GraphicsCommandRecorder::texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) {
	m_interface.texture_barrier(texture, old_layout, new_layout);
	return *this;
}

GraphicsCommandRecorder& GraphicsCommandRecorder::render_pass(
	const Texture& color,
	Option<const Texture&> depth,
	FunctionRef<void(RenderPassRecorder&)> callable
) {
	m_interface.begin_render_pass(color, depth);
	RenderPassRecorder recorder(m_interface);
	callable(recorder);
	m_interface.end_render_pass();
	return *this;
}

RenderPassRecorder& RenderPassRecorder::clear_color(LinearColor color) {
	m_interface.clear_color(color);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::clear_depth_stencil(f32 depth, u8 stencil) {
	m_interface.clear_depth_stencil(depth, stencil);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::set_pipeline(const GraphicsPipeline& pipeline) {
	m_interface.set_pipeline(pipeline);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::set_vertices(const Buffer& buffer) {
	ASSERT((buffer.usage() & BU_Vertex) != 0);
	m_interface.set_vertices(buffer);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::set_indices(const Buffer& buffer) {
	ASSERT((buffer.usage() & BU_Index) != 0);
	m_interface.set_indices(buffer);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::push_constants(const void* ptr) {
	m_interface.push_constant(ptr);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::draw(usize vertex_count, usize first_vertex) {
	m_interface.draw(vertex_count, first_vertex);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::draw_index(usize index_count, usize first_index) {
	m_interface.draw_indexed(index_count, first_index);
	return *this;
}

GPU_NAMESPACE_END