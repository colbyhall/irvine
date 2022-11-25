// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

template <typename T>
class NonNull {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	INLINE constexpr NonNull(T* ptr);

	// Accessors 
	INLINE operator T*() const;
	INLINE operator void*() const;
	INLINE T* operator ->() const;
	INLINE T& operator *() const;
private:
	T* m_ptr;
};

template <>
class NonNull<void> {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	INLINE constexpr NonNull(void* ptr);

	// Accessor
	INLINE operator void*() const;

private:
	void* m_ptr;
};

template <>
class NonNull<void const> {
public:
	NonNull() = delete;
	NonNull(NullPtr) = delete;
	INLINE constexpr NonNull(void const* ptr);

	// Accessor
	INLINE operator void const*() const;

private:
	void const* m_ptr;
};

#include <core/containers/non_null.inl>
