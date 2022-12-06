// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.hpp>
#include <gpu/d3d12/d3d12_shader.hpp>

GPU_NAMESPACE_BEGIN

VertexShader VertexShader::make(Array<u8>&& binary, Array<InputParameter>&& input_parameters) {
	auto& context = Context::the();

	Option<Shared<VertexShaderInterface, SMode::Atomic>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = make_shared<D3D12VertexShader, SMode::Atomic>(
			core::forward<Array<u8>>(binary),
			core::forward<Array<InputParameter>>(input_parameters)
			);
		break;
	}

	return VertexShader{ interface.unwrap() };
}

PixelShader PixelShader::make(Array<u8>&& binary) {
	auto& context = Context::the();

	Option<Shared<PixelShaderInterface, SMode::Atomic>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = make_shared<D3D12PixelShader, SMode::Atomic>(core::forward<Array<u8>>(binary));
		break;
	}

	return PixelShader{ interface.unwrap() };
}

GPU_NAMESPACE_END
