// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/memory.h>
#include <core/non_copyable.h>
#include <core/sync/atomic.h>

CORE_NAMESPACE_BEGIN

// Source: Dmitry Vyukov's MPMC
// http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
template <typename T>
class MPMCQueue final : private NonCopyable {
	struct CacheLinePad {
		u8 internal[64];
		CacheLinePad() : internal() {}
	};
	struct Cell {
		Atomic<int> sequence;
		Option<T> data;
	};

public:
	static MPMCQueue make(usize size);
	inline MPMCQueue(MPMCQueue<T>&& move) noexcept;
	inline MPMCQueue& operator=(MPMCQueue<T>&& move) noexcept;
	~MPMCQueue();

	bool push(T&& t) const;
	inline bool push(const T& t) const;
	Option<T> pop() const;

private:
	inline explicit MPMCQueue(Cell* buffer, int size) :
		m_buffer(buffer),
		m_buffer_mask(size - 1),
		m_enqueue_pos(0),
		m_dequeue_pos(0) {}

	CacheLinePad pad0;
	Cell* m_buffer;
	int m_buffer_mask;
	CacheLinePad pad1;
	Atomic<int> m_enqueue_pos;
	CacheLinePad pad2;
	Atomic<int> m_dequeue_pos;
	CacheLinePad pad3;
};

CORE_NAMESPACE_END

#include <core/sync/mpmc.inl>

using core::MPMCQueue;
