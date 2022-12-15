// Copyright Colby Hall. All Rights Reserved.

#include <game/ecs/component.hpp>

void hash(Hasher& hasher, const game::ComponentId& id) {
	hash(hasher, (u64)id);
}
