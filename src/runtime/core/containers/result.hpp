// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>
#include <core/templates/type_traits.hpp>

SUPPRESS_WARNINGS_STD_BEGIN
#include <new>
SUPPRESS_WARNINGS_STD_END

CORE_NAMESPACE_BEGIN

template <typename T, typename E>
class Result {
public:
	FORCE_INLINE Result(T&& t) : m_set(true), m_ok(true) {
		auto* p = &m_data[0];
		new (p) T(core::forward<T>(t));
	}

	FORCE_INLINE Result(E&& e) : m_set(true), m_ok(false) {
		auto* p = &m_data[0];
		new (p) E(core::forward<E>(e));
	}

	FORCE_INLINE bool is_ok() const { return m_ok; }
	explicit FORCE_INLINE operator bool() const { return is_ok(); }

	FORCE_INLINE T unwrap() {
		ASSERT(is_ok());
		m_set = false;

		auto* p = reinterpret_cast<T*>(&m_data[0]);
		return core::move(*p);
	}

	FORCE_INLINE E unwrap_err() {
		ASSERT(!is_ok());
		m_set = false;
		auto* p = reinterpret_cast<E*>(&m_data[0]);
		return core::move(*p);
	}

	~Result() { /* TODO */ }
private:
	u8 m_set : 1;
	u8 m_ok : 1; // Results technically can also be null after move

	union Internal {
		T t;
		E e;
		~Internal() {} // This prevents warning C4624
	};
	alignas(Internal) u8 m_data[sizeof(Internal)];
};

CORE_NAMESPACE_END