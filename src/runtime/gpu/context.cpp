// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/option.hpp>

#include <gpu/d3d12/d3d12_context.hpp>

GPU_NAMESPACE_BEGIN

static Option<Context> g_context = nullptr;

const Context& Context::the() {
	return g_context.as_ref().unwrap();
}

void init() {
	Unique<ContextInterface> interface = make_unique<D3D12Context>();
	g_context = Context(core::move(interface));
	g_context.as_mut().unwrap().m_interface->post_init();
}

GPU_NAMESPACE_END
