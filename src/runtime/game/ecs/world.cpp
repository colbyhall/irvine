// Copyright Colby Hall. All Rights Reserved.

#include <game/ecs/world.hpp>

GAME_NAMESPACE_BEGIN

EntityMut World::spawn() {
	const auto entity = m_entites.insert(Entity {});
	return EntityMut {
		*this,
		entity
	};
}

Option<EntityRef> World::get(EntityId entity) const {
	if (m_entites.contains(entity))
	{
		return EntityRef {
			*this,
			entity
		};
	}

	return nullptr;
}

Option<EntityMut> World::get_mut(EntityId entity) {
	if (m_entites.contains(entity))
	{
		return EntityMut {
			*this,
			entity
		};
	}

	return nullptr;
}


GAME_NAMESPACE_END
