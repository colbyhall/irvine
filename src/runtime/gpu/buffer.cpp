// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.hpp>
#include <gpu/d3d12/d3d12_buffer.hpp>

GPU_NAMESPACE_BEGIN

Buffer Buffer::make(
	BufferUsage usage,
	BufferKind kind,
	usize len,
	usize stride
) {
	auto& context = Context::the();

	Option<Shared<BufferInterface, SMode::Atomic>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = make_shared<D3D12Buffer, SMode::Atomic>(
			usage,
			kind,
			len,
			stride
		);
		break;
	}

	return Buffer { interface.unwrap() };
}

GPU_NAMESPACE_END
