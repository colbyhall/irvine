// Copyright Colby Hall. All Rights Reserved.

#include <core/memory.h>
#include <core/containers/wstring.h>

#include <dxc/dxc.h>

SUPPRESS_WARNING_PUSH
SUPPRESS_WARNINGS
MSVC_SUPPRESS_WARNING(4820)
MSVC_SUPPRESS_WARNING(4265)
MSVC_SUPPRESS_WARNING(4365)
MSVC_SUPPRESS_WARNING(4514)
MSVC_SUPPRESS_WARNING(5264)
MSVC_SUPPRESS_WARNING(5204)
MSVC_SUPPRESS_WARNING(5220)
MSVC_SUPPRESS_WARNING(5220)
MSVC_SUPPRESS_WARNING(4986)
MSVC_SUPPRESS_WARNING(4062)

#include <core/platform/windows.h>

#include <dxc/dxcapi.h>
#include <dxc/d3d12shader.h>

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using Microsoft::WRL::ComPtr;

DXC_NAMESPACE_BEGIN

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
static void throw_if_failed(HRESULT hr) {
	if (FAILED(hr))
	{
		ASSERT(false, "DXC Error");
	}
}

Result<Output, String> compile(const Input& input) {
	ComPtr<IDxcUtils> utils;
	throw_if_failed(DxcCreateInstance(
		CLSID_DxcUtils,
		IID_PPV_ARGS(utils.GetAddressOf())
	));

	DxcBuffer source = {};
	source.Ptr = *input.source;
	source.Size = input.source.len();

	ComPtr<IDxcCompiler3> compiler;
	throw_if_failed(DxcCreateInstance(
		CLSID_DxcCompiler,
		IID_PPV_ARGS(compiler.GetAddressOf())
	));

	Array<LPCWSTR> args;

	// entry point
	args.push(L"-E");
	WString wide_main;
	wide_main.reserve(input.main.len() + 16);
	wide_main.push(input.main);
	args.push(wide_main.ptr());

	// target profile
	args.push(L"-T");
	switch (input.type) {
	case ShaderType::Vertex:
		args.push(L"vs_6_2");
		break;
	case ShaderType::Pixel:
		args.push(L"ps_6_2");
		break;
	}

	args.push(L"-Qstrip_debug");
	args.push(L"-Qstrip_reflect");

	args.push(DXC_ARG_WARNINGS_ARE_ERRORS); //-WX
	// args.push(DXC_ARG_DEBUG); //-Zi
	// args.push(DXC_ARG_PACK_MATRIX_ROW_MAJOR);

	ComPtr<IDxcResult> result;
	throw_if_failed(compiler->Compile(
		&source,
		&args[0],
		(u32)args.len(),
		nullptr,
		IID_PPV_ARGS(result.GetAddressOf())
	));

	if (result->HasOutput(DXC_OUT_ERRORS))
	{
		ComPtr<IDxcBlobUtf8> output;
		throw_if_failed(result->GetOutput(
			DXC_OUT_ERRORS,
			IID_PPV_ARGS(output.GetAddressOf()),
			nullptr
		));

		if (output->GetBufferSize() > 0) {
			String error;
			error.reserve(output->GetBufferSize());
			error.set_len(output->GetBufferSize());
			core::copy(*error, output->GetBufferPointer(), error.len());
			OutputDebugStringA(*error);
			// return error;
		}
	}

	Output shader_output;

	if (result->HasOutput(DXC_OUT_REFLECTION)) {
		ComPtr<IDxcBlob> output;
		throw_if_failed(result->GetOutput(
			DXC_OUT_REFLECTION,
			IID_PPV_ARGS(output.GetAddressOf()),
			nullptr
		));
		DxcBuffer reflection_buffer = {};
		reflection_buffer.Ptr = output->GetBufferPointer();
		reflection_buffer.Size = output->GetBufferSize();

		ComPtr<ID3D12ShaderReflection> reflection;
		throw_if_failed(utils->CreateReflection(
			&reflection_buffer,
			IID_PPV_ARGS(reflection.GetAddressOf())
		));

		D3D12_SHADER_DESC shader_desc;
		throw_if_failed(reflection->GetDesc(&shader_desc));

		shader_output.input_parameters.reserve(shader_desc.InputParameters);

		for (UINT i = 0; i < shader_desc.InputParameters; ++i) {
			D3D12_SIGNATURE_PARAMETER_DESC param_desc;
			throw_if_failed(reflection->GetInputParameterDesc(i, &param_desc));

			String semantic_name;
			semantic_name.push(param_desc.SemanticName);

			const u32 num_bits = core::count_ones(param_desc.Mask);

			gpu::Primitive primitive = gpu::Primitive::Int32;
			switch (param_desc.ComponentType) {
			case D3D_REGISTER_COMPONENT_UINT32:
				primitive = gpu::Primitive::Uint32;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				primitive = gpu::Primitive::Int32;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				switch (num_bits) {
				case 1:
					primitive = gpu::Primitive::Float32;
					break;
				case 2:
					primitive = gpu::Primitive::Vec2f32;
					break;
				case 3:
					primitive = gpu::Primitive::Vec3f32;
					break;
				case 4:
					primitive = gpu::Primitive::Vec4f32;
					break;
				}
				break;
			}

			gpu::InputParameter input_parameter = {
				param_desc.SemanticIndex,
				core::move(semantic_name),
				primitive
			};
			shader_output.input_parameters.push(core::move(input_parameter));
		}
	}

	ComPtr<IDxcBlob> output;
	throw_if_failed(result->GetOutput(
		DXC_OUT_OBJECT,
		IID_PPV_ARGS(output.GetAddressOf()),
		nullptr
	));

	Array<u8> binary;
	binary.reserve(output->GetBufferSize());
	binary.set_len(output->GetBufferSize());
	core::copy(&binary[0], output->GetBufferPointer(), binary.len());

	shader_output.binary = core::move(binary);

	return shader_output;
}

DXC_NAMESPACE_END

SUPPRESS_WARNING_POP
