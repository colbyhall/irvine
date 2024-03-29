// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/function.h>
#include <core/containers/slice.h>
#include <core/containers/shared.h>

#include <gpu/gpu.h>

GPU_NAMESPACE_BEGIN

BITFLAG(BufferUsage) {
	BU_TransferSrc = (1 << 0),
	BU_TransferDst = (1 << 1),
	BU_Vertex = (1 << 2),
	BU_Index = (1 << 3),
	BU_Constant = (1 << 4),
};

enum class BufferKind {
	Storage,
	Upload,
	Download,
};

class BufferInterface : private NonCopyable {
public:
	virtual BufferUsage usage() const = 0;
	virtual BufferKind kind() const = 0;
	virtual usize len() const = 0;
	virtual usize stride() const = 0;
	virtual void map(FunctionRef<void (Slice<u8>)>& func) = 0;
	virtual ~BufferInterface() {}
};

class Buffer {
public:
	static Buffer make(
		BufferUsage usage,
		BufferKind kind,
		usize len,
		usize stride
	);

	inline BufferUsage usage() const { return m_interface->usage(); }
	inline BufferKind kind() const { return m_interface->kind(); }
	inline usize len() const { return m_interface->len(); }
	inline usize stride() const { return m_interface->stride(); }
	inline void map(FunctionRef<void (Slice<u8>)> func) { m_interface->map(func); }
	inline Buffer clone() const { return m_interface.clone(); }

	template <typename T = BufferInterface>
	T const& interface() const {
		static_assert(core::is_base_of<BufferInterface, T>, "T is not derived of BufferInterface");
		return static_cast<const T&>(*m_interface);
	}

private:
	Buffer(Shared<BufferInterface, SMode::Atomic>&& interface) : m_interface(core::move(interface)) { }

	Shared<BufferInterface, SMode::Atomic> m_interface;
};

GPU_NAMESPACE_END
