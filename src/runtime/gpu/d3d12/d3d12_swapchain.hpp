// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gpu/swapchain.hpp>
#include <gpu/d3d12/d3d12_context.hpp>

GPU_NAMESPACE_BEGIN

class D3D12Swapchain : public SwapchainInterface {
	static constexpr usize frame_count = 2;

public:
	explicit D3D12Swapchain(void* window_handle);

	FORCE_INLINE D3D12Swapchain(D3D12Swapchain&& move) noexcept :
		m_swapchain(move.m_swapchain),
		m_back_buffers(core::move(move.m_back_buffers)),
		m_current(move.m_current),
		m_fence(move.m_fence),
		m_fence_event(move.m_fence_event),
		m_fence_value(move.m_fence_value) {}


    // SwapchainInterface
	const Texture& back_buffer() const override;
	void present() override;
	void wait_for_previous() override;
    // ~SwapchainInterface

private:
	ComPtr<IDXGISwapChain3> m_swapchain;
	Array<Texture> m_back_buffers;
	u8 m_current;

	ComPtr<ID3D12Fence> m_fence;
	HANDLE m_fence_event;
	u64 m_fence_value;
};

GPU_NAMESPACE_END