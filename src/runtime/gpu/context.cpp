// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/option.hpp>

#include <gpu/d3d12/d3d12_context.hpp>

GPU_NAMESPACE_BEGIN

const Context& Context::the() {
	static Option<Context> context = nullptr;
	if (!context) {
		Unique<ContextInterface> interface = make_unique<D3D12Context>();
		context = Context(core::move(interface));
		context.as_mut().unwrap().m_interface->post_init();
	}
	return context.as_ref().unwrap();
}

GPU_NAMESPACE_END
