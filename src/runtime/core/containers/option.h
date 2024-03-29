// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/memory.h>
#include <core/templates/enable_if.h>
#include <core/templates/type_traits.h>

CORE_NAMESPACE_BEGIN

// Alias nullptr for readability
constexpr NullPtr none = nullptr;

template <typename T, typename Enable = void>
class Option {
public:
	Option() = default;
	inline constexpr Option(NullPtr) : m_set(false), m_data() {}
	inline Option(T&& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(core::forward<T>(t));
	}
	inline Option(const T& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(t);
	}

	// Non trivially copyable Option's do not allow the use of the copy
	// operators
	Option(const Option<T>& copy) = delete;
	Option& operator=(const Option<T>& copy) = delete;

	inline Option(Option<T>&& move) noexcept : m_set(move.m_set) {
		core::copy(m_data, move.m_data, sizeof(T));
		move.m_set = false;
	}

	inline Option& operator=(Option<T>&& m) noexcept {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
		}
		m_set = true;
		m.m_set = false;
		core::copy(m_data, m.m_data, sizeof(T));
		return *this;
	}

	inline Option& operator=(T&& t) {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
		}
		auto* p = m_data;
		m_set = true;
		new (p) T(core::forward<T>(t));
		return *this;
	}

	inline bool is_set() const { return m_set; }
	inline explicit operator bool() const { return is_set(); }

	inline T unwrap() {
		ASSERT(is_set(), "Value must be set to be unwrapped");
		m_set = false;

		auto* p = reinterpret_cast<T*>(&m_data[0]);
		return core::move(*p);
	}

	inline Option<T&> as_mut() {
		if (is_set()) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			return Option<T&>(*p);
		}

		return Option<T&>();
	}

	inline Option<T const&> as_ref() const {
		if (is_set()) {
			auto* p = reinterpret_cast<T const*>(&m_data[0]);
			return Option<T const&>(*p);
		}

		return Option<T const&>();
	}

	~Option() {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
			m_set = false;
		}
	}

private:
	bool m_set = false;
	alignas(T) u8 m_data[sizeof(T)] = {};
};

template <typename T>
class Option<T, EnabledIf<core::is_trivially_copyable<T>>> {
public:
	Option() = default;
	inline constexpr Option(NullPtr) : m_set(false), m_data() {}
	inline Option(const T& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(t);
	}

	inline bool is_set() const { return m_set; }
	inline explicit operator bool() const { return is_set(); }

	inline T unwrap() const {
		ASSERT(is_set(), "Value must be set to be unwrapped");

		// Do not reset m_set for trivially copyable types

		auto* p = reinterpret_cast<const T*>(&m_data[0]);
		return *p;
	}

	inline Option<T&> as_mut() {
		if (is_set()) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			return Option<T&>(*p);
		}
		else {
			return Option<T&>();
		}
	}

	inline Option<T const&> as_ref() const {
		if (is_set()) {
			auto* p = reinterpret_cast<T const*>(&m_data[0]);
			return Option<T const&>(*p);
		}
		else {
			return Option<T const&>();
		}
	}

	~Option() {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
			m_set = false;
		}
	}

private:
	bool m_set = false;
	alignas(T) u8 m_data[sizeof(T)] = {};
};

template <typename T>
class Option<T&> {
public:
	explicit Option() = default;
	inline constexpr Option(NullPtr) : m_ptr(nullptr) {}
	inline constexpr Option(T& t) : m_ptr(&t) { }

	inline bool is_set() const { return m_ptr != nullptr; }
	inline explicit operator bool() const { return is_set(); }

	inline T& unwrap() {
		ASSERT(is_set());
		return *m_ptr;
	}

private:
	T* m_ptr = nullptr;
};

CORE_NAMESPACE_END

// Export Option out of core namespace
using core::Option;
