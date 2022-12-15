// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <game/game.hpp>

GAME_NAMESPACE_BEGIN

class ComponentId;

constexpr usize archetype_id_width = 64; // max components is archetype_id_width * sizeof(u64) = 512

class ArchetypeId {
public:
    ArchetypeId() = default;

    ArchetypeId& with(const ComponentId& id);
    bool contains(const ComponentId& id) const;
    bool contains(const ArchetypeId& id) const;
    
    FORCE_INLINE bool operator==(const ArchetypeId& rhs) const {
        for (usize i = 0; i < archetype_id_width; ++i) {
            if (m_bitsets[i] != rhs.m_bitsets[i]) return false;
        }
        return true;
    }

    FORCE_INLINE bool operator!=(const ArchetypeId& rhs) const { return !(*this == rhs); }

private:
    u64 m_bitsets[archetype_id_width] = {};
};

GAME_NAMESPACE_END