// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/allocators.h>
#include <core/containers/option.h>
#include <core/containers/slice.h>

CORE_NAMESPACE_BEGIN

template <typename T>
class Array : private NonCopyable {
public:
    FORCE_INLINE Array() = default;
	explicit FORCE_INLINE Array(Slice<const T> slice);
	FORCE_INLINE Array(Array<T>&& move) noexcept;
	FORCE_INLINE Array& operator=(Array<T>&& move) noexcept;
	~Array();

	FORCE_INLINE usize len() const { return m_len; }
	FORCE_INLINE usize cap() const { return m_cap; }

	FORCE_INLINE bool is_empty() const { return len() == 0; }
	explicit operator bool() const { return !is_empty(); }
	FORCE_INLINE bool is_valid_index(usize index) const { return index < len(); }

	FORCE_INLINE operator Slice<T>() { return { m_ptr, m_len }; }
	FORCE_INLINE operator Slice<T const>() const { return { m_ptr, m_len }; }

	FORCE_INLINE T* begin() { return m_ptr; }
	FORCE_INLINE T* end() { return m_ptr + m_len; }
	FORCE_INLINE const T* cbegin() const { return m_ptr; }
	FORCE_INLINE const T* cend() const { return m_ptr + m_len; }

	FORCE_INLINE T& operator[](usize index);
	FORCE_INLINE const T& operator[](usize index) const;

	FORCE_INLINE Option<T&> last_mut();
	FORCE_INLINE Option<T const&> last() const;

	// FIXME: Delete the items that are lost and initialize any gained to default
	FORCE_INLINE void set_len(usize len);

	void reserve(usize amount);
	void insert(usize index, T&& item);
	void insert(usize index, const T& item);
	T remove(usize index);
	FORCE_INLINE usize push(T&& item);
	FORCE_INLINE usize push(const T& item);
	FORCE_INLINE Option<T> pop();

private:
    T* m_ptr = nullptr;
    usize m_len = 0;
    usize m_cap = 0;

};

CORE_NAMESPACE_END

#include <core/containers/array.inl>

// Export Array out of core namespace
using core::Array;
