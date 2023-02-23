// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.h>

#include <gpu/buffer.h>
#include <gpu/graphics_command_list.h>
#include <gpu/texture.h>
#include <gpu/d3d12/d3d12.h>

GPU_NAMESPACE_BEGIN

D3D12_RESOURCE_STATES layout_to_resource_states(Layout layout);

class D3D12GraphicsCommandList : public GraphicsCommandListInterface {
public:
	explicit D3D12GraphicsCommandList();

    FORCE_INLINE D3D12GraphicsCommandList(D3D12GraphicsCommandList&& move) noexcept :
        m_command_list(move.m_command_list),
        m_bound_color_buffer(core::move(move.m_bound_color_buffer)),
        m_bound_depth_buffer(core::move(move.m_bound_depth_buffer)),
        m_textures_in_use(core::move(move.m_textures_in_use)),
        m_buffers_in_use(core::move(move.m_buffers_in_use))
	{
        //m_command_list.Reset();
        m_bound_color_buffer = nullptr;
        m_bound_depth_buffer = nullptr;
	}

    // GraphicsCommandListInterface
	void begin_recording() override;
	void copy_buffer_to_texture(const Texture& dst, const Buffer& src) override;
	void copy_buffer_to_buffer(const Buffer& dst, const Buffer& src) override;
	void texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) override;
	void begin_render_pass(const Texture& color, Option<const Texture&> depth) override;
	void set_scissor(Option<Aabb2f32> scissor) override;
	void clear_color(const LinearColor& color) override;
	void clear_depth_stencil(f32 depth, u8 stencil) override;
	void set_pipeline(const GraphicsPipeline& pipeline) override;
	void set_vertices(const Buffer& buffer) override;
	void set_indices(const Buffer& buffer) override;
	void push_constant(const void* ptr) override;
	void draw(usize vertex_count, usize first_vertex) override;
	void draw_indexed(usize index_count, usize first_index) override;
	void end_render_pass() override;
	void end_recording() override;
	void submit() override;
    // ~GraphicsCommandListInterface

private:
	ComPtr<ID3D12GraphicsCommandList> m_command_list;

	Option<D3D12_CPU_DESCRIPTOR_HANDLE> m_bound_color_buffer;
	Option<D3D12_CPU_DESCRIPTOR_HANDLE> m_bound_depth_buffer;

	Array<Texture> m_textures_in_use;
	Array<Buffer> m_buffers_in_use;
};

GPU_NAMESPACE_END
