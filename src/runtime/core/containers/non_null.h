// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

CORE_NAMESPACE_BEGIN

template <typename T>
class NonNull {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	inline constexpr NonNull(T* ptr) : m_ptr(ptr) { ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr"); }

	// Accessors
	inline operator T*() const { return m_ptr; }
	inline operator void*() const { return m_ptr; }
	inline T* operator ->() const { return m_ptr; }
	inline T& operator *() const { return *m_ptr; }

private:
	T* m_ptr;
};

template <>
class NonNull<void> {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	inline constexpr NonNull(void* ptr) : m_ptr(ptr) { ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr"); }

	// Accessor
	inline operator void*() const { return m_ptr; }

private:
	void* m_ptr;
};

template <>
class NonNull<void const> {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	inline constexpr NonNull(void const* ptr) : m_ptr(ptr) { ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr"); }

	// Accessor
	inline operator void const*() const { return m_ptr; }

private:
	void const* m_ptr;
};

CORE_NAMESPACE_END

// Export NonNull out of core namespace
using core::NonNull;
