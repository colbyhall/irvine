// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/option.h>

SUPPRESS_WARNINGS_STD_BEGIN
#include <atomic>
SUPPRESS_WARNINGS_STD_END

CORE_NAMESPACE_BEGIN

enum class Order : u8 {
	Relaxed,
	SequentiallyConsistent
};

template <typename T>
class Atomic final : private NonCopyable {
public:
	Atomic() noexcept = default;
	constexpr Atomic(T desired) noexcept : m_atomic(desired) {}

	inline Atomic(Atomic&& rhs) noexcept
		: m_atomic(rhs.m_atomic.load(std::memory_order_relaxed)) {}
	inline Atomic& operator=(Atomic&& rhs) noexcept {
		const auto order = std::memory_order_relaxed;
		m_atomic.store(rhs.m_atomic.load(order), order);
	}

	inline void store(T desired, Order order = Order::SequentiallyConsistent) const noexcept;
	inline T load(Order order = Order::SequentiallyConsistent) const noexcept;
	inline T exchange(T desired, Order order = Order::SequentiallyConsistent) const noexcept;

	inline Option<T> compare_exchange_weak(
		T expected,
		T desired,
		Order order = Order::SequentiallyConsistent
	) const noexcept;

	inline Option<T> compare_exchange_strong(
		T expected,
		T desired,
		Order order = Order::SequentiallyConsistent
	) const noexcept;

	inline T fetch_add(T arg, Order order = Order::SequentiallyConsistent) const noexcept;
	inline T fetch_sub(T arg, Order order = Order::SequentiallyConsistent) const noexcept;
	inline T fetch_and(T arg, Order order = Order::SequentiallyConsistent) const noexcept;
	inline T fetch_or(T arg, Order order = Order::SequentiallyConsistent) const noexcept;
	inline T fetch_xor(T arg, Order order = Order::SequentiallyConsistent) const noexcept;

private:
	inline std::memory_order to_std(Order order) const {
		static const std::memory_order convert[] = {
				std::memory_order_relaxed,
				std::memory_order_seq_cst
		};
		return convert[(u8)order];
	}

	mutable std::atomic<T> m_atomic;
};

CORE_NAMESPACE_END

#include <core/sync/atomic.inl>

// Export Atomic out of core namespace
using core::Atomic;
