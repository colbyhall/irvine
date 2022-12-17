// Copyright Colby Hall. All Rights Reserved.

#include <game/ecs/archetype_id.h>
#include <game/ecs/component.h>

GAME_NAMESPACE_BEGIN

ArchetypeId& ArchetypeId::with(const ComponentId& id) {
	const usize size = sizeof(m_bitsets);
	const auto slot = (u64)id & size;

	const auto index = slot / archetype_id_width;
	const auto shift = slot & archetype_id_width;
	m_bitsets[index] |= ((u64)1 << shift);

	return *this;
}

bool ArchetypeId::contains(const ComponentId& id) const {
	const usize size = sizeof(m_bitsets);
	const auto slot = (u64)id & size;

	const auto index = slot / archetype_id_width;
	const auto shift = slot & archetype_id_width;
	const auto shifted = ((u64)1 << shift);
	return (m_bitsets[index] & shifted) == shifted;
}

bool ArchetypeId::contains(const ArchetypeId& id) const {
	for (usize i = 0; i < archetype_id_width; ++i) {
		if ((m_bitsets[i] & id.m_bitsets[i]) != id.m_bitsets[i]) return false;
	}
	return true;
}

GAME_NAMESPACE_END
