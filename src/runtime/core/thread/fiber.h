// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/function.h>

CORE_NAMESPACE_BEGIN

class Fiber final : private NonCopyable {
public:
	using Handle = void*;
	using Function = core::Function<void()>;

	Fiber spawn(Function&& spawn);
	Fiber convert();

	FORCE_INLINE Fiber(Fiber&& move) noexcept
			: m_thread(move.m_thread), m_handle(move.m_handle){
		move.m_handle = nullptr;
	}

	FORCE_INLINE Fiber& operator=(Fiber&& move) noexcept {
		// FIXME: Is this the best way to do this
		Fiber to_destroy = core::move(*this);
		m_handle = move.m_handle;
		m_thread = move.m_thread;
		move.m_handle = nullptr;
		return *this;
	}

	~Fiber();

	void switch_to();

	FORCE_INLINE Handle handle() const { return m_handle; }

private:
	FORCE_INLINE explicit Fiber(bool thread, Handle handle) : m_thread(thread), m_handle(handle) {}

	bool m_thread;
	Handle m_handle;
};

CORE_NAMESPACE_END

using core::Fiber;
