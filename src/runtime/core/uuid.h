// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

CORE_NAMESPACE_BEGIN

class Uuid {
public:
	static const Uuid nil;
	static Uuid generate();

	bool operator==(const Uuid& right) const;
	bool operator!=(const Uuid& right) const { return !(*this == right); }

private:
	explicit inline constexpr Uuid(u32 a, u16 b, u16 c, u64 d) : m_a(a), m_b(b), m_c(c), m_d(d) {}

	u32 m_a;
	u16 m_b;
	u16 m_c;
	u64 m_d;
};

CORE_NAMESPACE_END

using core::Uuid;

