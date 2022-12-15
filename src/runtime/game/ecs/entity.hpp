// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/slot_map.hpp>

#include <game/ecs/archetype_id.hpp>

GAME_NAMESPACE_BEGIN

struct Entity {
	Option<ArchetypeId> archetype;
};

using Entities = SlotMap<Entity>;
using EntityId = SlotKey;

GAME_NAMESPACE_END
