// Copyright Colby Hall. All Rights Reserved.

#include <game/ecs/archetype.hpp>

GAME_NAMESPACE_BEGIN

void Archetype::spawn(EntityId entity) {
	auto pop = m_free_list.pop();
	if (pop.is_set()) {
		const auto index = pop.unwrap();
		m_entities[index] = entity;
		m_lookup.insert(entity, index);
	} else {
		const auto index = m_entities.len();
		m_entities.push(entity);
		m_lookup.insert(entity, (u32)index);
	}
}

void Archetype::transfer(EntityId entity, Archetype& dst) {
	const u32 src_index = m_lookup.find(entity).unwrap();

	// Setup entity in dst_archetype
	dst.spawn(entity);
	const u32 dst_index = dst.m_lookup.find(entity).unwrap();
	const auto dst_id = dst.id();

	// Transfer components between archetypes storage
	for (auto iter = m_components.iter_mut(); iter; ++iter) {
		const auto component_id = iter.key();
		auto& src_storage = iter.value();

		if (dst_id.contains(component_id)) {
			auto possible = dst.m_components.find_mut(component_id);
			Storage* dst_storage = nullptr;
			if (possible) {
				dst_storage = &*possible.unwrap();
			}
			// If the dst_archetype does not have a storage for this component type
			// make a new one from the src
			else {
				auto empty_storage = src_storage->make_empty();
				dst_storage = &*empty_storage;
				dst.m_components.insert(component_id, core::move(empty_storage));
			}
			src_storage->transfer(src_index, dst_index, *dst_storage);
		}
		// Destroy the component if the dst_archetype doesnt support it
		else {
			src_storage->remove(src_index);
		}
	}

	// Remove entity from src_archetype
	m_entities[src_index] = nullptr;
	m_lookup.remove(entity);
}

GAME_NAMESPACE_END
