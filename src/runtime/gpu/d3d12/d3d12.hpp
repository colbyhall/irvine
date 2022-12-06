// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/platform/windows.hpp>

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

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
inline void throw_if_failed(HRESULT hr) {
	if (FAILED(hr))
	{
		ASSERT(false, "D3D12 Crash");
	}
}

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>

SUPPRESS_WARNING_POP

#undef interface