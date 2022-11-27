// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/option.hpp>

SUPPRESS_WARNINGS_STD_BEGIN
#include <atomic>
SUPPRESS_WARNINGS_STD_END

CORE_NAMESPACE_BEGIN

enum class Order : u8 {
    Relaxed,
    SequentiallyConsistent
};

template <typename T>
class Atomic : NonCopyable {
public:
    Atomic() noexcept = default;
    constexpr Atomic(T desired) noexcept : m_atomic(desired) {}

    FORCE_INLINE Atomic(Atomic&& rhs) noexcept 
        : m_atomic(rhs.m_atomic.load(std::memory_order_relaxed)) {}
    FORCE_INLINE Atomic& operator=(Atomic&& rhs) noexcept {
        const auto order = std::memory_order_relaxed;
        m_atomic.store(rhs.m_atomic.load(order), order);
    }

    FORCE_INLINE void store(T desired, Order order = Order::SequentiallyConsistent) const noexcept {
        m_atomic.store(desired, to_std(order));
    }

    FORCE_INLINE T load(Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.load(to_std(order));
    }

    FORCE_INLINE T exchange(T desired, Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.exchange(desired, to_std(order));
    }

    FORCE_INLINE Option<T> compare_exchange_weak(
        T expected,
        T desired,
        Order order = Order::SequentiallyConsistent
    ) const noexcept {
        if (m_atomic.compare_exchange_weak(expected, desired, to_std(order))) {
            return expected;
        }
        return Option<T> {};
    }

    FORCE_INLINE Option<T> compare_exchange_strong(
        T expected,
        T desired,
        Order order = Order::SequentiallyConsistent
    ) const noexcept {
        if (m_atomic.compare_exchange_strong(expected, desired, to_std(order))) {
            return expected;
        }
        return Option<T> {};
    }

    FORCE_INLINE T fetch_add(T arg, Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.fetch_add(arg, to_std(order));
    }

    FORCE_INLINE T fetch_sub(T arg, Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.fetch_sub(arg, to_std(order));
    }

    FORCE_INLINE T fetch_and(T arg, Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.fetch_and(arg, to_std(order));
    }

    FORCE_INLINE T fetch_or(T arg, Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.fetch_or(arg, to_std(order));
    }

    FORCE_INLINE T fetch_xor(T arg, Order order = Order::SequentiallyConsistent) const noexcept {
        return m_atomic.fetch_xor(arg, to_std(order));
    }

private:
    FORCE_INLINE std::memory_order to_std(Order order) const {
        static const std::memory_order convert[] = {
                std::memory_order_relaxed,
                std::memory_order_seq_cst
        };
        return convert[(u8)order];
    }

    mutable std::atomic<T> m_atomic;
};

CORE_NAMESPACE_END

// Export Atomic out of core namespace
using core::Atomic;