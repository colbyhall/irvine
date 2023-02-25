// Copyright Colby Hall. All Rights Reserved.

#include <gpu/d3d12/d3d12_swapchain.h>
#include <gpu/d3d12/d3d12_texture.h>

GPU_NAMESPACE_BEGIN

D3D12Swapchain::D3D12Swapchain(void* window_handle) {
	auto& context = Context::the().interface<D3D12Context>();
	m_hwnd = (HWND)window_handle;

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	const Vec2u32 size = { (u32)(rect.right - rect.left), (u32)(rect.bottom - rect.top) };

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = D3D12Swapchain::frame_count;
	desc.Width = size.width;
	desc.Height = size.height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapchain1;
	throw_if_failed(context.factory->CreateSwapChainForHwnd(
		context.queue.Get(),
		(HWND)window_handle,
		&desc,
		nullptr,
		nullptr,
		&swapchain1
	));

	throw_if_failed(context.factory->MakeWindowAssociation(
		(HWND)window_handle,
		DXGI_MWA_NO_ALT_ENTER
	));

	throw_if_failed(swapchain1.As(&m_swapchain));
	m_current = (u8)m_swapchain->GetCurrentBackBufferIndex();

	throw_if_failed(context.device->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_fence)
	));
	m_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	ASSERT(m_fence_event);
	m_fence_value = 1;

	for (int i = 0; i < D3D12Swapchain::frame_count; ++i) {
		ComPtr<ID3D12Resource> resource;
		throw_if_failed(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&resource)));

		const Vec3u32 buffer_size = { size.width, size.height, 1 };

		Shared<TextureInterface, SMode::Atomic> interface = make_shared<D3D12Texture, SMode::Atomic>(
			TU_ColorAttachment | TU_Swapchain,
			Format::RGBA_U8,
			buffer_size,
			resource
		);
		m_back_buffers.push(Texture(core::move(interface)));
	}

	wait_for_previous();
}

const Texture& D3D12Swapchain::back_buffer() const {
	return m_back_buffers[m_current];
}

void D3D12Swapchain::present() {
	throw_if_failed(m_swapchain->Present(1, 0));
	wait_for_previous();

	auto& context = Context::the().interface<D3D12Context>();
	throw_if_failed(context.command_allocator->Reset());
}

void D3D12Swapchain::wait_for_previous() {
	auto& context = Context::the().interface<D3D12Context>();

	const auto fence_value = m_fence_value;
	throw_if_failed(context.queue->Signal(m_fence.Get(), fence_value));
	m_fence_value += 1;

	// Wait until the previous frame is finished and then flush the work from the graphics queue
	if (m_fence->GetCompletedValue() < fence_value) {
		throw_if_failed(m_fence->SetEventOnCompletion(fence_value, m_fence_event));
		WaitForSingleObject(m_fence_event, INFINITE);
	}
	context.flush_queue();

	m_current = (u8)m_swapchain->GetCurrentBackBufferIndex();
}

void D3D12Swapchain::resize() {
	m_back_buffers.reset();

	wait_for_previous();

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	const Vec2u32 size = { (u32)(rect.right - rect.left), (u32)(rect.bottom - rect.top) };

	throw_if_failed(m_swapchain->ResizeBuffers(0, size.width, size.height, DXGI_FORMAT_UNKNOWN, 0));

	for (int i = 0; i < D3D12Swapchain::frame_count; ++i) {
		ComPtr<ID3D12Resource> resource;
		throw_if_failed(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&resource)));

		const Vec3u32 buffer_size = { size.width, size.height, 1 };

		Shared<TextureInterface, SMode::Atomic> interface = make_shared<D3D12Texture, SMode::Atomic>(
			TU_ColorAttachment | TU_Swapchain,
			Format::RGBA_U8,
			buffer_size,
			resource
		);
		m_back_buffers.push(Texture(core::move(interface)));
	}

	wait_for_previous();
}

GPU_NAMESPACE_END
