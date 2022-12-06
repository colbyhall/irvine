// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gpu/shader.hpp>
#include <gpu/d3d12/d3d12.hpp>

GPU_NAMESPACE_BEGIN

class D3D12VertexShader : public gpu::VertexShaderInterface {
public:
	explicit D3D12VertexShader(Array<u8>&& binary, Array<gpu::InputParameter>&& input_parameters)
		: m_binary(core::forward<Array<u8>>(binary)), m_input_parameters(core::forward< Array<gpu::InputParameter>>(input_parameters)) {}

	Slice<u8 const> binary() const override { return m_binary; }
	Slice<gpu::InputParameter const> input_parameters() const override { return m_input_parameters; }

	Array<u8> m_binary;
	Array<gpu::InputParameter> m_input_parameters;
};

class D3D12PixelShader : public gpu::PixelShaderInterface {
public:
	explicit D3D12PixelShader(Array<u8>&& binary)
		: m_binary(core::forward<Array<u8>>(binary)) {}

	Slice<u8 const> binary() const override { return m_binary; }

	Array<u8> m_binary;
};

GPU_NAMESPACE_END