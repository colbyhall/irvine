// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.hpp>
#include <core/non_copyable.hpp>
#include <core/containers/string_view.hpp>

#include <game/game.hpp>

GAME_NAMESPACE_BEGIN

class Component : public NonCopyable {
public:
    virtual ~Component() {}
};

class ComponentId {
public:
    FORCE_INLINE constexpr explicit ComponentId(u64 value) : m_value(value) {}

    template <typename T>
    static ComponentId of() {
        static_assert(core::is_base_of<Component, T>, "T must be derived from Component");

        StringView name = T::name();
        FNV1Hasher hasher = {};
        hash(hasher, name);
        const auto type_hash = hasher.finish();

        return ComponentId { type_hash };
    }

    FORCE_INLINE explicit operator u64() const { return m_value; }

    FORCE_INLINE bool operator==(const ComponentId& rhs) const { return m_value == rhs.m_value; }
    FORCE_INLINE bool operator!=(const ComponentId& rhs) const { return m_value != rhs.m_value; }

private:
    u64 m_value;
};

#define DECLARE_COMPONENT(n)                                        \
    static StringView name() { return #n; }

GAME_NAMESPACE_END

void hash(Hasher& hasher, const game::ComponentId& id);