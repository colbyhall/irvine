// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/non_null.h>
#include <core/templates/enable_if.h>
#include <core/templates/type_traits.h>

CORE_NAMESPACE_BEGIN

/// Non owning reference to an array
template <typename T>
class Slice {
public:
	Slice() = default;
	inline constexpr Slice(T* ptr, usize len) : m_ptr(ptr), m_len(len) {
		ASSERT(
			ptr != nullptr || (ptr == nullptr && len == 0),
			"If we have no ptr we must not have len"
		);
	}

	inline constexpr Slice(const Slice<T>& c) : m_ptr(c.m_ptr), m_len(c.m_len) {}
	inline constexpr Slice<T>& operator=(const Slice<T>& c) {
		m_ptr = c.m_ptr;
		m_len = c.m_len;
		return *this;
	}

	inline Slice(Slice<T>&& m) noexcept : m_ptr(m.m_ptr), m_len(m.m_len) {
		m.m_ptr = nullptr;
		m.m_len = 0;
	}
	inline Slice<T>& operator=(Slice<T>&& m) noexcept {
		m_ptr = m.m_ptr;
		m_len = m.m_len;

		m.m_ptr = nullptr;
		m.m_len = 0;

		return *this;
	}

	inline usize len() const { return m_len; }
	inline bool is_empty() const { return m_len == 0; }
	inline bool is_valid_index(usize index) const { return index < m_len; }

	inline T* begin() const { return m_ptr; }
	inline T* end() const { return m_ptr + m_len; }

	inline T const* cbegin() const { return m_ptr; }
	inline T const* cend() const { return m_ptr + m_len; }

	template <typename = EnabledIf<!is_const<T>>>
	inline T& operator[](usize index) {
		ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}
	inline T const& operator[](usize index) const {
		ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}

	inline explicit operator bool() const { return !is_empty(); }

	template <typename = EnabledIf<!is_const<T>>>
	inline explicit operator Slice<const T>() const { return Slice<const T>{ m_ptr, m_len }; }

	/// Shrinks the slice by amount. Returns new len
	inline usize shrink(usize amount) {
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
