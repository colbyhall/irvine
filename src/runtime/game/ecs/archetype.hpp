// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/hash_map.hpp>

#include <game/game.hpp>

GAME_NAMESPACE_BEGIN

class Archetype : private NonCopyable {
public:
private:
    HashMap<u64, void*> m_components;
};

GAME_NAMESPACE_END