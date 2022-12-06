// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gpu/buffer.hpp>
#include <gpu/d3d12/d3d12_context.hpp>

GPU_NAMESPACE_BEGIN

class D3D12Buffer : public BufferInterface {
public:
	explicit D3D12Buffer(
		BufferUsage usage, 
		BufferKind kind, 
		usize len, 
		usize stride
	);

	FORCE_INLINE D3D12Buffer(D3D12Buffer&& move) noexcept :
		m_usage(move.m_usage),
		m_kind(move.m_kind),
		m_len(move.m_len),
		m_stride(move.m_stride),
		m_resource(core::move(move.m_resource))
	{
		move.m_resource = nullptr;
	}

    // BufferInterface
	BufferUsage usage() const override { return m_usage; }
	BufferKind kind() const override { return m_kind; }
	usize len() const override { return m_len; }
	usize stride() const override { return m_stride; }
    void map(FunctionRef<void (Slice<u8>)>& func) override;
    // ~BufferInterface

	BufferUsage m_usage;
	BufferKind m_kind;
	usize m_len;
	usize m_stride;

	ComPtr<ID3D12Resource> m_resource;
};

GPU_NAMESPACE_END