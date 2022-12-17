// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gpu/texture.h>
#include <gpu/d3d12/d3d12_context.h>

GPU_NAMESPACE_BEGIN

DXGI_FORMAT format_to_dxgi(Format format);

class D3D12Texture : public TextureInterface {
public:
	explicit D3D12Texture(
		TextureUsage usage,
		Format format,
		const Vec3u32& size,
		ComPtr<ID3D12Resource> resource = nullptr
	);

	FORCE_INLINE D3D12Texture(D3D12Texture&& move) noexcept :
		m_resource(core::move(move.m_resource)),
		m_usage(move.m_usage),
		m_format(move.m_format),
		m_size(move.m_size),
		m_rtv_handle(move.m_rtv_handle),
		m_dsv_handle(move.m_dsv_handle),
		m_bindless_handle(move.m_bindless_handle)
	{
		move.m_resource = nullptr;
		move.m_rtv_handle = {};
		move.m_dsv_handle = {};
		move.m_bindless_handle = {};
	}

    // TextureInterface
    TextureUsage usage() const override { return m_usage; }
	Format format() const override { return m_format; }
	Vec3u32 size() const override { return m_size; }
	u32 bindless() const override { return m_bindless_handle.index; }
	~D3D12Texture() override;
    // ~TextureInterface

	ComPtr<ID3D12Resource> m_resource;
	TextureUsage m_usage;
	Format m_format;
	Vec3u32 m_size;

	D3D12_CPU_DESCRIPTOR_HANDLE m_rtv_handle = {};
	D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_handle = {};
	D3D12BindlessHandle m_bindless_handle = {};
};

GPU_NAMESPACE_END
