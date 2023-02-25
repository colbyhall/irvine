// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.h>
#include <gui/gui.h>

GUI_NAMESPACE_BEGIN

class Id {
public:
	template <typename T>
	FORCE_INLINE explicit Id(const T& t) {
		FNV1Hasher hasher;
		hash(hasher, t);
		m_value = hasher.finish();
	}

	FORCE_INLINE static Id from_raw(u64 value) {
		Id result = {};
		result.m_value = value;
		return result;
	}

	FORCE_INLINE bool operator==(const Id& rhs) const { return m_value == rhs.m_value; }
	FORCE_INLINE bool operator!=(const Id& rhs) const { return m_value != rhs.m_value; }
	FORCE_INLINE operator u64() const { return m_value; }

private:
	Id() = default;

	u64 m_value;
};

GUI_NAMESPACE_END

void hash(Hasher& hasher, const gui::Id& value);
