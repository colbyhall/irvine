// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.h>
#include <core/non_copyable.h>
#include <core/containers/string_view.h>

#include <game/game.h>

GAME_NAMESPACE_BEGIN

class Component {
public:
	virtual ~Component() {}
};

class ComponentId {
public:
	inline constexpr explicit ComponentId(u64 value) : m_value(value) {}

	template <typename T>
	static ComponentId of() {
		static_assert(core::is_base_of<Component, T>, "T must be derived from Component");

		StringView name = T::name();
		FNV1Hasher hasher = {};
		hash(hasher, name);
		const auto type_hash = hasher.finish();

		return ComponentId { type_hash };
	}

	inline explicit operator u64() const { return m_value; }

	inline bool operator==(const ComponentId& rhs) const { return m_value == rhs.m_value; }
	inline bool operator!=(const ComponentId& rhs) const { return m_value != rhs.m_value; }

private:
	u64 m_value;
};

#define DECLARE_COMPONENT(n)                                        \
	static StringView name() { return #n; }

GAME_NAMESPACE_END

void hash(Hasher& hasher, const game::ComponentId& id);
