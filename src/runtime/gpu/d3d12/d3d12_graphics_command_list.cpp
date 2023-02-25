// Copyright Colby Hall. All Rights Reserved.

#include <gpu/d3d12/d3d12_buffer.h>
#include <gpu/d3d12/d3d12_context.h>
#include <gpu/d3d12/d3d12_graphics_command_list.h>
#include <gpu/d3d12/d3d12_graphics_pipeline.h>
#include <gpu/d3d12/d3d12_texture.h>

GPU_NAMESPACE_BEGIN

D3D12_RESOURCE_STATES layout_to_resource_states(Layout layout) {
	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	SUPPRESS_WARNING_PUSH
	MSVC_SUPPRESS_WARNING(4061) // Not all switch cases are explicitly handled
	switch (layout) {
		case Layout::Undefined:
			break;
		case Layout::General:
			state = D3D12_RESOURCE_STATE_GENERIC_READ;
			break;
		case Layout::ColorAttachment:
			state = D3D12_RESOURCE_STATE_RENDER_TARGET;
			break;
		// case Layout::DepthAttachment:
		// case Layout::TransferSrc:
		case Layout::TransferDst:
			state = D3D12_RESOURCE_STATE_COPY_DEST;
			break;
		// case Layout::ShaderReadOnly:
		case Layout::Present:
			state = D3D12_RESOURCE_STATE_PRESENT;
			break;
		default:
			ASSERT(false, "TODO");
			break;
	}
	SUPPRESS_WARNING_POP

	return state;
}

D3D12GraphicsCommandList::D3D12GraphicsCommandList() {
	auto& context = Context::the().interface<D3D12Context>();

	throw_if_failed(context.device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		context.command_allocator.Get(),
		nullptr,
		IID_PPV_ARGS(&m_command_list)
	));
	throw_if_failed(m_command_list->Close());
}

void D3D12GraphicsCommandList::begin_recording() {
	auto& context = Context::the().interface<D3D12Context>();

	// throw_if_failed(context.command_allocator->Reset());
	throw_if_failed(m_command_list->Reset(context.command_allocator.Get(), nullptr));
	m_command_list->SetGraphicsRootSignature(context.root_signature.Get());

	auto* heap_ptr = context.bindless_heap.heap().Get();
	m_command_list->SetDescriptorHeaps(1, &heap_ptr);

	m_command_list->SetGraphicsRootDescriptorTable(
		(UINT)D3D12BindlessDescriptorHeap::texture2d_index,
		context.bindless_heap.gpu_texture2d_table()
	);
}

void D3D12GraphicsCommandList::copy_buffer_to_texture(const Texture& dst, const Buffer& src) {
	const auto& dst_interface = dst.interface<D3D12Texture>();
	D3D12_TEXTURE_COPY_LOCATION dst_location = {};
	dst_location.pResource = dst_interface.m_resource.Get();

	const auto& src_interface = src.interface<D3D12Buffer>();
	D3D12_TEXTURE_COPY_LOCATION src_location = {};
	src_location.pResource = src_interface.m_resource.Get();

	src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src_location.PlacedFootprint.Footprint.Format = format_to_dxgi(dst.format());
	src_location.PlacedFootprint.Footprint.Width = dst_interface.m_size.width;
	src_location.PlacedFootprint.Footprint.Height = dst_interface.m_size.height;
	src_location.PlacedFootprint.Footprint.Depth = dst_interface.m_size.depth;
	src_location.PlacedFootprint.Footprint.RowPitch = (UINT)(format_size_in_bytes(dst_interface.m_format) * dst_interface.m_size.width);

	m_command_list->CopyTextureRegion(
		&dst_location,
		0,
		0,
		0,
		&src_location,
		nullptr
	);

	m_textures_in_use.push(dst.clone());
	m_buffers_in_use.push(src.clone());
}

void D3D12GraphicsCommandList::copy_buffer_to_buffer(const Buffer& dst, const Buffer& src) {
	ASSERT(false, "TODO");
}

void D3D12GraphicsCommandList::texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) {
	const auto& interface = texture.interface<D3D12Texture>();

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.pResource = interface.m_resource.Get();
	barrier.Transition.StateBefore = layout_to_resource_states(old_layout);
	barrier.Transition.StateAfter = layout_to_resource_states(new_layout);
	m_command_list->ResourceBarrier(1, &barrier);

	m_textures_in_use.push(texture.clone());
}

void D3D12GraphicsCommandList::begin_render_pass(const Texture& color, Option<const Texture&> depth) {
	m_textures_in_use.push(color.clone());

	const D3D12_CPU_DESCRIPTOR_HANDLE* depth_handle = nullptr;
	if (depth) {
		auto& depth_texture = depth.unwrap();
		auto& interface = depth_texture.interface<D3D12Texture>();
		depth_handle = &interface.m_dsv_handle;
		m_textures_in_use.push(depth_texture.clone());
	}

	const auto& interface = color.interface<D3D12Texture>();
	m_command_list->OMSetRenderTargets(
		1,
		&interface.m_rtv_handle,
		0,
		depth_handle
	);

	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (f32)color.size().width;
	viewport.Height = (f32)color.size().height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_command_list->RSSetViewports(1, &viewport);

	D3D12_RECT rect = {};
	rect.left = 0;
	rect.top = 0;
	rect.bottom = (LONG)color.size().height;
	rect.right = (LONG)color.size().width;
	m_command_list->RSSetScissorRects(1, &rect);

	m_bound_color_buffer = interface.m_rtv_handle;
	if (depth_handle) m_bound_depth_buffer = *depth_handle;
}

void D3D12GraphicsCommandList::set_scissor(Option<Aabb2f32> scissor) {
	ASSERT(false, "TODO");
}

void D3D12GraphicsCommandList::clear_color(const LinearColor& color) {
	const auto handle = m_bound_color_buffer.as_ref().unwrap();
	const f32 clear_color0[] = { color.r, color.g, color.g, color.a };
	m_command_list->ClearRenderTargetView(
		handle,
		clear_color0,
		0,
		nullptr
	);
}

void D3D12GraphicsCommandList::clear_depth_stencil(f32 depth, u8 stencil) {
	const auto handle = m_bound_depth_buffer.as_ref().unwrap();
	m_command_list->ClearDepthStencilView(
		handle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		depth,
		stencil,
		0,
		nullptr
	);
}

void D3D12GraphicsCommandList::set_pipeline(const GraphicsPipeline& pipeline) {
	auto& interface = pipeline.interface<D3D12GraphicsPipeline>();
	m_command_list->SetPipelineState(interface.m_pipeline.Get());
	m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D12GraphicsCommandList::set_vertices(const Buffer& buffer) {
	auto& interface = buffer.interface<D3D12Buffer>();

	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = (UINT)interface.m_resource->GetGPUVirtualAddress();
	view.StrideInBytes = (UINT)interface.m_stride;
	view.SizeInBytes = (UINT)(interface.m_len * interface.m_stride);
	m_command_list->IASetVertexBuffers(0, 1, &view);
	m_buffers_in_use.push(buffer.clone());
}

void D3D12GraphicsCommandList::set_indices(const Buffer& buffer) {
	auto& interface = buffer.interface<D3D12Buffer>();

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = (UINT)interface.m_resource->GetGPUVirtualAddress();
	view.SizeInBytes = (UINT)(interface.m_len * interface.m_stride);
	view.Format = DXGI_FORMAT_R32_UINT; // All index buffers must be of u32
	m_command_list->IASetIndexBuffer(&view);
	m_buffers_in_use.push(buffer.clone());
}

void D3D12GraphicsCommandList::push_constant(const void* ptr) {
	m_command_list->SetGraphicsRoot32BitConstants(0, 16, ptr, 0);
}

void D3D12GraphicsCommandList::draw(usize vertex_count, usize first_vertex) {
	m_command_list->DrawInstanced(
		(UINT)vertex_count,
		1,
		(UINT)first_vertex,
		0
	);
}

void D3D12GraphicsCommandList::draw_indexed(usize index_count, usize first_index) {
	m_command_list->DrawIndexedInstanced(
		(UINT)index_count,
		1,
		(UINT)first_index,
		0,
		0
	);
}

void D3D12GraphicsCommandList::end_render_pass() {
	if (m_bound_color_buffer) {
		const auto ignore0 = m_bound_color_buffer.unwrap();
	}

	if (m_bound_depth_buffer) {
		const auto ignore1 = m_bound_depth_buffer.unwrap();
	}
}

void D3D12GraphicsCommandList::end_recording() {
	throw_if_failed(m_command_list->Close());
}

void D3D12GraphicsCommandList::submit(const GraphicsCommandList& command_list) {
	auto& context = Context::the().interface<D3D12Context>();

	ID3D12CommandList* ppCommandLists[] = { m_command_list.Get() };
	context.queue->ExecuteCommandLists(1, ppCommandLists);

	ComPtr<ID3D12Fence> fence;
	throw_if_failed(context.device->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence)
	));
	throw_if_failed(context.queue->Signal(fence.Get(), 1));

	context.flush_queue();

	context.work_queue.push(D3D12QueuedWork{
		fence,
		command_list.clone(),
	});
}

GPU_NAMESPACE_END
