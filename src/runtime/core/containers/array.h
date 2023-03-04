// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/option.h>
#include <core/containers/slice.h>

CORE_NAMESPACE_BEGIN

template <typename T>
class Array : private NonCopyable {
public:
	inline Array() = default;
	explicit inline Array(Slice<const T> slice);
	inline Array(Array<T>&& move) noexcept;
	inline Array& operator=(Array<T>&& move) noexcept;
	~Array();

	inline usize len() const { return m_len; }
	inline usize cap() const { return m_cap; }

	inline bool is_empty() const { return len() == 0; }
	explicit operator bool() const { return !is_empty(); }
	inline bool is_valid_index(usize index) const { return index < len(); }

	inline operator Slice<T>() { return { m_ptr, m_len }; }
	inline operator Slice<T const>() const { return { m_ptr, m_len }; }

	inline T* begin() { return m_ptr; }
	inline T* end() { return m_ptr + m_len; }
	inline const T* cbegin() const { return m_ptr; }
	inline const T* cend() const { return m_ptr + m_len; }

	inline T& operator[](usize index);
	inline const T& operator[](usize index) const;

	inline Option<T&> last_mut();
	inline Option<T const&> last() const;

	inline void set_len(usize len);

	void reserve(usize amount);
	void insert(usize index, T&& item);
	inline void insert(usize index, const T& item);
	T remove(usize index);
	void reset();
	inline usize push(T&& item);
	inline usize push(const T& item);
	inline Option<T> pop();

private:
	T* m_ptr = nullptr;
	usize m_len = 0;
	usize m_cap = 0;

};

CORE_NAMESPACE_END

#include <core/containers/array.inl>

// Export Array out of core namespace
using core::Array;
