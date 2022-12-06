// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.hpp>
#include <gpu/d3d12/d3d12_swapchain.hpp>

GPU_NAMESPACE_BEGIN

Swapchain Swapchain::make(void* window_handle) {
	auto& context = Context::the();

	Option<Unique<SwapchainInterface>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = make_unique<D3D12Swapchain>(window_handle);
		break;
	}

	return Swapchain { interface.unwrap() };
}

GPU_NAMESPACE_END
