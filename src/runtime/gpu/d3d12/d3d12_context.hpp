// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/platform/library.hpp>
#include <core/async/atomic.hpp>

#include <gpu/context.hpp>
#include <gpu/texture.hpp>
#include <gpu/d3d12/d3d12.hpp>

GPU_NAMESPACE_BEGIN

class D3D12FreeSlotDescriptorHeap {
public:
	explicit D3D12FreeSlotDescriptorHeap() = default;
	explicit D3D12FreeSlotDescriptorHeap(
		ComPtr<ID3D12Device1> device,
		D3D12_DESCRIPTOR_HEAP_TYPE type,
		usize cap, 
		bool shader_visible
	);

	D3D12_CPU_DESCRIPTOR_HANDLE alloc() const;
	void free(D3D12_CPU_DESCRIPTOR_HANDLE handle) const;

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;
	Array<Atomic<bool>> m_free_slots;
	usize m_size = 0;
	usize m_cap = 0;
};

struct D3D12BindlessHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle;
	u32 index;
};

class D3D12BindlessDescriptorHeap {
public:
	static inline constexpr usize texture2d_count = 1024;
	static inline constexpr usize texture2d_index = 1;

	explicit D3D12BindlessDescriptorHeap() = default;
	explicit D3D12BindlessDescriptorHeap(ComPtr<ID3D12Device1> device);

	D3D12BindlessHandle alloc_texture2d() const;
	void free_texture2d(D3D12BindlessHandle handle) const;
	D3D12_GPU_DESCRIPTOR_HANDLE gpu_texture2d_table() const;

	FORCE_INLINE ComPtr<ID3D12DescriptorHeap> heap() const { return m_heap; }

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;
	Array<Atomic<bool>> m_free_slots;
	usize m_size;
};

typedef HRESULT(__stdcall* PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)(const D3D12_ROOT_SIGNATURE_DESC* root_signature_desc, D3D_ROOT_SIGNATURE_VERSION version, ID3DBlob** blob, ID3DBlob** error_blob);

typedef HRESULT(__stdcall* PFN_DXGI1_3_CREATE_DXGI_FACTORY2)(
	UINT   Flags,
	REFIID riid,
	void** ppFactory
);

class D3D12Context : public ContextInterface {
    using FnCreateDevice = PFN_D3D12_CREATE_DEVICE;
    using FnSerializeRootSignature = PFN_D3D12_SERIALIZE_ROOT_SIGNATURE;
    using FnGetDebugInterface = PFN_D3D12_GET_DEBUG_INTERFACE;
	using FnCreateDXGIFactory2 = PFN_DXGI1_3_CREATE_DXGI_FACTORY2;

public:
    explicit D3D12Context();

	FORCE_INLINE D3D12Context(D3D12Context&& move) noexcept :	
		d3d12_library(core::move(move.d3d12_library)),
		create_device(move.create_device),
		serialize_root_signature(move.serialize_root_signature),
		get_debug_interface(move.get_debug_interface),
		device(move.device),
		factory(move.factory),
		queue(move.queue),
		command_allocator(move.command_allocator),
#ifdef BUILD_DEBUG
		debug_interface(move.debug_interface),
#endif
		root_signature(move.root_signature),
		rtv_heap(core::move(move.rtv_heap)),
		dsv_heap(core::move(move.dsv_heap)),
		bindless_heap(core::move(move.bindless_heap)),
		bindless_texture(core::move(move.bindless_texture)) { }

    // ContextInterface
    Backend backend() const override { return Backend::D3D12; }
    void post_init() override;
    // ~ContextInterface

    Library d3d12_library;
    FnCreateDevice create_device;
    FnSerializeRootSignature serialize_root_signature;
	FnGetDebugInterface get_debug_interface;

    ComPtr<ID3D12Device1> device;
	ComPtr<IDXGIFactory4> factory;
	ComPtr<ID3D12CommandQueue> queue;
    ComPtr<ID3D12CommandAllocator> command_allocator;

#ifdef BUILD_DEBUG
	ComPtr<ID3D12Debug> debug_interface;
#endif

    ComPtr<ID3D12RootSignature> root_signature;
	D3D12FreeSlotDescriptorHeap rtv_heap;
	D3D12FreeSlotDescriptorHeap dsv_heap;
	
	D3D12BindlessDescriptorHeap bindless_heap;
	Option<Texture> bindless_texture;

};

GPU_NAMESPACE_END
