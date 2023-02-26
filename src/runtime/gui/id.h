// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.h>
#include <gui/gui.h>

GUI_NAMESPACE_BEGIN

class Id {
public:
	template <typename T>
	inline explicit Id(const T& t) {
		FNV1Hasher hasher;
		hash(hasher, t);
		m_value = hasher.finish();
	}

	inline static Id from_raw(u64 value) {
		Id result = {};
		result.m_value = value;
		return result;
	}

	inline bool operator==(const Id& rhs) const { return m_value == rhs.m_value; }
	inline bool operator!=(const Id& rhs) const { return m_value != rhs.m_value; }
	inline operator u64() const { return m_value; }

private:
	Id() = default;

	u64 m_value;
};

GUI_NAMESPACE_END

void hash(Hasher& hasher, const gui::Id& value);
