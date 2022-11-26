// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
class NonNull {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	FORCE_INLINE constexpr NonNull(T* ptr) : m_ptr(ptr) { ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr"); }

	// Accessors 
	FORCE_INLINE operator T*() const { return m_ptr; }
	FORCE_INLINE operator void*() const { return m_ptr; }
	FORCE_INLINE T* operator ->() const { return m_ptr; }
	FORCE_INLINE T& operator *() const { return *m_ptr; }

private:
	T* m_ptr;
};

template <>
class NonNull<void> {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	FORCE_INLINE constexpr NonNull(void* ptr) : m_ptr(ptr) { ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr"); }

	// Accessor
	FORCE_INLINE operator void*() const { return m_ptr; }

private:
	void* m_ptr;
};

template <>
class NonNull<void const> {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	FORCE_INLINE constexpr NonNull(void const* ptr) : m_ptr(ptr) { ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr"); }

	// Accessor
	FORCE_INLINE operator void const*() const { return m_ptr; }

private:
	void const* m_ptr;
};

CORE_NAMESPACE_END

// Export NonNull out of core namespace
using core::NonNull;