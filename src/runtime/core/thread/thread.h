// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/function.h>

CORE_NAMESPACE_BEGIN

class JoinHandle;

class Thread final : private NonCopyable {
public:
	using Id = u32;
	using Handle = void*;
	using Function = core::Function<void()>;

	static JoinHandle spawn(bool start, Thread::Function&& function);
	static Thread current();

	FORCE_INLINE Thread(Thread&& move) noexcept
		: m_handle(move.m_handle), m_id(move.m_id) {
		move.m_handle = nullptr;
		move.m_id = 0;
	}
	FORCE_INLINE Thread& operator=(Thread&& move) noexcept {
		m_handle = move.m_handle;
		m_id = move.m_id;
		return *this;
	}

	void resume() const;

	FORCE_INLINE Handle handle() const { return m_handle; }
	FORCE_INLINE Id id() const { return m_id; }

private:
	FORCE_INLINE explicit Thread(void* handle, Id id) : m_handle(handle), m_id(id) {}
	friend JoinHandle;

	Handle m_handle;
	Id m_id;
};

class JoinHandle {
public:
	~JoinHandle();

	bool join();
	FORCE_INLINE const Thread& thread() const { return m_thread; }

private:
	FORCE_INLINE explicit JoinHandle(void* handle, Thread::Id id) : m_thread(handle, id), m_joined(false) {}

	friend class Thread;

	Thread m_thread;
	bool m_joined;
};

CORE_NAMESPACE_END

using core::Thread;
using core::JoinHandle;
