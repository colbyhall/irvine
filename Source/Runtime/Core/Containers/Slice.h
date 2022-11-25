// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <Core/Containers/NonNull.h>

template <typename T>
class Slice {
public:
	Slice() = default;
	INLINE constexpr Slice(T* ptr, usize len);

	INLINE usize len() const;
	INLINE bool is_empty() const;
	INLINE bool is_valid_index(usize index) const;

	INLINE T* begin();
	INLINE T* end();
	INLINE T const* cbegin() const;
	INLINE T const* cend() const;

	INLINE T& operator[](usize index);
	INLINE T const& operator[](usize index) const;

	INLINE explicit operator bool() const;
	INLINE explicit operator Slice<const T>() const;

	/// Shrinks the slice by amount. Returns new len
	INLINE usize shrink(usize amount);

private:
	T* m_ptr = nullptr;
	usize m_len = 0;
};

template <typename T>
class Slice<T const> {
public:
	Slice() = default;
	INLINE constexpr Slice(T const* ptr, usize len);

	INLINE usize len() const;
	INLINE bool is_empty() const;
	INLINE bool is_valid_index(usize index) const;

	INLINE T const* cbegin() const;
	INLINE T const* cend() const;

	INLINE T const& operator[](usize index) const;
	INLINE explicit operator bool() const;

	INLINE usize shrink(usize amount);

private:
	T const* m_ptr = nullptr;
	usize m_len = 0;
};

#include <Core/Containers/Slice.inl>
