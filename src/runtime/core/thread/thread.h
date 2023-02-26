// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/time.h>
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

	inline Thread(Thread&& move) noexcept
		: m_handle(move.m_handle), m_id(move.m_id) {
		move.m_handle = nullptr;
		move.m_id = 0;
	}
	inline Thread& operator=(Thread&& move) noexcept {
		m_handle = move.m_handle;
		m_id = move.m_id;
		return *this;
	}

	void set_affinity(u32 core) const;
	void resume() const;

	inline Handle handle() const { return m_handle; }
	inline Id id() const { return m_id; }

private:
	inline explicit Thread(void* handle, Id id) : m_handle(handle), m_id(id) {}
	friend JoinHandle;

	Handle m_handle;
	Id m_id;
};

class JoinHandle {
public:
	bool join();
	inline const Thread& thread() const { return m_thread; }

private:
	inline explicit JoinHandle(void* handle, Thread::Id id) : m_thread(handle, id), m_joined(false) {}

	friend class Thread;

	Thread m_thread;
	bool m_joined;
};

void sleep(const Duration& duration);

// FIXME: Find a better place for this when appropriate
u32 logical_core_count();

CORE_NAMESPACE_END

using core::Thread;
using core::JoinHandle;
