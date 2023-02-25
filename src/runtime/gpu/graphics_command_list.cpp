// Copyright Colby Hall. All Rights Reserved.

#include <gpu/buffer.h>
#include <gpu/context.h>
#include <gpu/d3d12/d3d12_graphics_command_list.h>

GPU_NAMESPACE_BEGIN

GraphicsCommandList GraphicsCommandList::record(FunctionRef<void(GraphicsCommandRecorder&)> callable) {
	auto& context = Context::the();

	Option<Shared<GraphicsCommandListInterface, SMode::Atomic>> interface = nullptr;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = make_shared<D3D12GraphicsCommandList, SMode::Atomic>();
		break;
	}

	auto result = GraphicsCommandList { interface.unwrap() };
	result.m_interface->begin_recording();
	GraphicsCommandRecorder recorder(*result.m_interface);
	callable(recorder);
	result.m_interface->end_recording();
	return result;
}

void GraphicsCommandList::submit() {
	GraphicsCommandListInterface& interface = *m_interface;
	interface.submit(*this);
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
