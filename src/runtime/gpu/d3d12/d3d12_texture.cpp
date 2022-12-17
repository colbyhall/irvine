// Copyright Colby Hall. All Rights Reserved.

#include <gpu/d3d12/d3d12_texture.h>

GPU_NAMESPACE_BEGIN

DXGI_FORMAT format_to_dxgi(Format format) {
	DXGI_FORMAT dxgi_format = DXGI_FORMAT_UNKNOWN;

	SUPPRESS_WARNING_PUSH
	MSVC_SUPPRESS_WARNING(4061) // Not all switch cases are explicitly handled
	switch (format) {
	case Format::R_U8:
		dxgi_format = DXGI_FORMAT_R8_UNORM;
		break;
		// RGB_U8,
		// RGB_U8_SRGB,
	case Format::RGBA_U8:
		dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case Format::RGBA_U8_SRGB:
		dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		break;
	case Format::RGBA_F16:
		dxgi_format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		break;
	case Format::RGBA_F32:
		dxgi_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
		// BGR_U8_SRGB,
		// Depth16,
	case Format::Depth24_Stencil8:
		dxgi_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	default:
		ASSERT(false, "Unsupported format type");
		break;
	}
	SUPPRESS_WARNING_POP

	return dxgi_format;
}

D3D12Texture::D3D12Texture(
	TextureUsage usage,
	Format format,
	const Vec3u32& size,
	ComPtr<ID3D12Resource> resource
) : m_usage(usage), m_format(format), m_size(size) {
	ASSERT(size.width > 0);
	ASSERT(size.height > 0);
	ASSERT(size.depth > 0);

	auto& context = Context::the().interface<D3D12Context>();

	D3D12_RESOURCE_DIMENSION dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN;
	if (size.width > 1) {
		dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		if (size.height > 1) {
			dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			if (size.depth > 1) {
				dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}
		}
	}
	ASSERT(dimension != D3D12_RESOURCE_DIMENSION_UNKNOWN);

	const DXGI_FORMAT dxgi_format = format_to_dxgi(format);

	const bool color_attachment = (usage & TU_ColorAttachment) != 0;
	const bool depth_attachment = (usage & TU_DepthAttachment) != 0;
	const bool sampled = (usage & TU_Sampled) != 0;

	if (resource == nullptr) {
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = dimension;
		desc.Width = size.width;
		desc.Height = size.height;
		desc.DepthOrArraySize = (UINT16)size.depth;
		desc.MipLevels = 1;
		desc.Format = dxgi_format;
		desc.SampleDesc.Count = 1;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (color_attachment) {
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
		if (depth_attachment) {
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		D3D12_RESOURCE_STATES initial_state = D3D12_RESOURCE_STATE_GENERIC_READ;
		if (depth_attachment) {
			initial_state = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		}

		D3D12_HEAP_PROPERTIES heap = {};
		heap.Type = D3D12_HEAP_TYPE_DEFAULT;

		bool optimized_clear = color_attachment;
		optimized_clear |= depth_attachment;

		D3D12_CLEAR_VALUE clear = {};
		D3D12_CLEAR_VALUE* pclear = nullptr;
		if (optimized_clear) {
			clear.Color[3] = 1.f;
			clear.Format = dxgi_format;
			clear.DepthStencil.Depth = 1.f;
			pclear = &clear;
		}

		throw_if_failed(context.device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			initial_state,
			pclear,
			IID_PPV_ARGS(&m_resource)
		));
	} else {
		m_resource = resource;
	}

	if (color_attachment) {
		m_rtv_handle = context.rtv_heap.alloc();
		context.device->CreateRenderTargetView(m_resource.Get(), nullptr, m_rtv_handle);
	}
	if (depth_attachment) {
		m_dsv_handle = context.dsv_heap.alloc();
		context.device->CreateDepthStencilView(m_resource.Get(), nullptr, m_dsv_handle);
	}
	if (sampled) {
		m_bindless_handle = context.bindless_heap.alloc_texture2d();
		context.device->CreateShaderResourceView(m_resource.Get(), nullptr, m_bindless_handle.descriptor_handle);
	}
}

D3D12Texture::~D3D12Texture() {
	auto& context = Context::the().interface<D3D12Context>();

	if (m_rtv_handle.ptr) {
		context.rtv_heap.free(m_rtv_handle);
		m_rtv_handle = {};
	}
	if (m_dsv_handle.ptr) {
		context.dsv_heap.free(m_dsv_handle);
		m_dsv_handle = {};
	}
	if (m_bindless_handle.index) {
		context.bindless_heap.free_texture2d(m_bindless_handle);
		m_bindless_handle = {};
	}
}

GPU_NAMESPACE_END
