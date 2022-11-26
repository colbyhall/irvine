// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/allocators.hpp>
#include <core/containers/option.hpp>
#include <core/containers/slice.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
class Array : private NonCopyable {
public:
    FORCE_INLINE Array() = default;
	FORCE_INLINE explicit Array(Slice<const T> slice) : m_ptr(nullptr), m_len(slice.len()), m_cap(0) {
		reserve(slice.len());
		for (int i = 0; i < slice.len(); ++i) {
			T copy = slice[i];
			new (m_ptr + i) T(core::move(copy));
		}
	}
	~Array() {
		if (m_ptr) {
			for (usize i = 0; i < m_len; ++i) {
				T& item = m_ptr[i];
				item.~T();
			}
			mem::free(m_ptr);
			m_ptr = nullptr;
		}
	}
	
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

	FORCE_INLINE T& operator[](usize index) {
		ASSERT(is_valid_index(index), "Index out of bounds");
		return m_ptr[index];
	}
	FORCE_INLINE const T& operator[](usize index) const {
		ASSERT(is_valid_index(index), "Index out of bounds");
		return m_ptr[index];
	}

	FORCE_INLINE Option<T&> last() {
		if (len() > 0) return m_ptr[len() - 1];
		return {};
	}
	FORCE_INLINE Option<T const&> last() const {
		if (len() > 0) return m_ptr[len() - 1];
		return {};
	}

	void reserve(usize amount) {
		// FIXME: Find proper way of allocating in larger blocks to prevent allocation per push
		const auto old_cap = m_cap;
		auto new_cap = old_cap + amount;
		while (m_cap < new_cap) {
			m_cap += new_cap >> 1;
			m_cap += 1;
		}

		// FIXME: Custom allocator support
		if (m_ptr == nullptr) {
			void* ptr = mem::alloc(mem::Layout::array<T>(m_cap));
			m_ptr = static_cast<T*>(ptr);
		}
		else {
			void* ptr = mem::realloc(m_ptr, mem::Layout::array<T>(old_cap), mem::Layout::array<T>(m_cap));
			m_ptr = static_cast<T*>(ptr);
		}
	}

	void insert(usize index, T&& item) {
		ASSERT(index <= m_len);
		if (len() == cap()) reserve(1);

		auto* src = m_ptr + index;
		if (index != len()) {
			mem::move(src + 1, src, (len() - index) * sizeof(T));
			mem::set(src, 0, sizeof(T));
		}

		new (src) T(core::forward<T>(item));
		m_len += 1;
	}

	FORCE_INLINE void insert(usize index, const T& item) {
		T copy = item;
		insert(index, core::move(copy));
	}

	T remove(usize index) {
		ASSERT(index < m_len);

		T result = core::move(m_ptr[index]);
		void* clear = &m_ptr[index];
		mem::set(clear, 0, sizeof(T));
		if (index < m_len - 1) {
			auto* src = m_ptr + index;
			mem::move(src, src + 1, (len() - index) * sizeof(T));
		}
		m_len -= 1;
		return result;
	}

	FORCE_INLINE usize push(T&& item) {
		const auto index = len();
		insert(index, core::move(item));
		return index;
	}
	FORCE_INLINE usize push(const T& item) {
		T copy = item;
		return push(core::move(copy));
	}

private:
    T* m_ptr = nullptr;
    usize m_len = 0;
    usize m_cap = 0;

};

CORE_NAMESPACE_END

// Export Array out of core namespace
using core::Array;