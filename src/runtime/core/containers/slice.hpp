// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/non_null.hpp>
#include <core/templates/enable_if.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

/// Non owning reference to a buffer of memory
template <typename T>
class Slice {
public:
	Slice() = default;
	FORCE_INLINE constexpr Slice(T* ptr, usize len) : m_ptr(ptr), m_len(len) { 
		ASSERT(ptr != nullptr || (ptr == nullptr && len == 0), "If we have no ptr we must not have len"); 
	}

	FORCE_INLINE constexpr Slice(const Slice<T>& c) : m_ptr(c.m_ptr), m_len(c.m_len) {}
	FORCE_INLINE constexpr Slice<T>& operator=(const Slice<T>& c) {
		m_ptr = c.m_ptr;
		m_len = c.m_len;
		return *this;
	}

	FORCE_INLINE Slice(Slice<T>&& m) : m_ptr(m.m_ptr), m_len(m.m_len) {
		m.m_ptr = nullptr;
		m.m_len = 0;
	}
	FORCE_INLINE Slice<T>& operator=(Slice<T>&& m) {
		m_ptr = m.m_ptr;
		m_len = m.m_len;

		m.m_ptr = nullptr;
		m.m_len = 0;

		return *this;
	}

	FORCE_INLINE usize len() const { return m_len; }
	FORCE_INLINE bool is_empty() const { return m_len == 0; }
	FORCE_INLINE bool is_valid_index(usize index) const { return index < m_len; }

	template <typename = EnabledIf<!is_const<T>>>
	FORCE_INLINE T* begin() const { return m_ptr; }
	template <typename = EnabledIf<!is_const<T>>>
	FORCE_INLINE T* end() const { return m_ptr + m_len; }

	FORCE_INLINE T const* cbegin() const { return m_ptr; }
	FORCE_INLINE T const* cend() const { return m_ptr + m_len; }

	template <typename = EnabledIf<!is_const<T>>>
	FORCE_INLINE T& operator[](usize index) {
		ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}
	FORCE_INLINE T const& operator[](usize index) const {
		ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}

	FORCE_INLINE explicit operator bool() const { return !is_empty(); }

	template <typename = EnabledIf<!is_const<T>>>
	FORCE_INLINE explicit operator Slice<const T>() const { return Slice<const T>{ m_ptr, m_len }; }

	/// Shrinks the slice by amount. Returns new len
	FORCE_INLINE usize shrink(usize amount) {
		ASSERT(amount <= m_len, "Can not shrink more than len");
		m_len -= amount;
		return len();
	}

private:
	T* m_ptr = nullptr;
	usize m_len = 0;
};

CORE_NAMESPACE_END

// Export Slice out of core namespace
using core::Slice;
