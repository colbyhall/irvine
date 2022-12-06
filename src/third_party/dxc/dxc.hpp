// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.hpp>
#include <core/containers/result.hpp>
#include <core/containers/string_view.hpp>

#include <gpu/shader.hpp>

// Begin the dxc namespace
#define DXC_NAMESPACE_BEGIN																    \
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace dxc {

// End the dxc namespace
#define DXC_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP

DXC_NAMESPACE_BEGIN

using gpu::ShaderType;
using gpu::InputParameter;

struct Input {
    StringView source;
    StringView main;
    ShaderType type;
};

struct Output {
    Array<u8> binary;
    Array<InputParameter> input_parameters;
};

Result<Output, String> compile(const Input& input);

DXC_NAMESPACE_END