// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/slot_map.h>

#include <game/ecs/archetype.h>
#include <game/ecs/entity.h>

GAME_NAMESPACE_BEGIN

class World;

class EntityRef : private NonCopyable {
public:
    FORCE_INLINE EntityRef(const World& world, EntityId entity) : m_world(&world), m_entity(entity) {}

    FORCE_INLINE EntityRef(EntityRef&& move) noexcept : m_world(move.m_world), m_entity(move.m_entity) {}
    FORCE_INLINE EntityRef& operator=(EntityRef&& t) noexcept {
        m_world = t.m_world;
        m_entity = t.m_entity;
        return *this;
    }

    template<typename T>
    Option<const T&> get() const;
    FORCE_INLINE EntityId id() const { return m_entity; }

private:
    const World* m_world;
    EntityId m_entity;
};

class EntityMut : private NonCopyable {
public:
    FORCE_INLINE EntityMut(World& world, EntityId entity) : m_world(&world), m_entity(entity) {}

    FORCE_INLINE EntityMut(EntityMut&& move) noexcept : m_world(move.m_world), m_entity(move.m_entity) {}
    FORCE_INLINE EntityMut& operator=(EntityMut&& t) noexcept {
        m_world = t.m_world;
        m_entity = t.m_entity;
        return *this;
    }

	template<typename T>
	EntityMut& insert(T&& component);
    template<typename T>
    Option<const T&> get() const;
    template<typename T>
    Option<T&> get_mut() const;

    FORCE_INLINE EntityId id() const { return m_entity; }

private:
    World* m_world;
    EntityId m_entity;
};

class World {
public:
    explicit World() = default;

    EntityMut spawn();

    Option<EntityRef> get(EntityId entity) const;
    Option<EntityMut> get_mut(EntityId entity);

private:
	friend class EntityRef;
	friend class EntityMut;

    Entities m_entites;
	Array<Archetype> m_archetypes;
};

template<typename T>
Option<const T&> EntityRef::get() const {
    auto& entity = m_world->m_entites.get(m_entity).unwrap();
    if (entity.archetype) {
        const auto archetype_id = entity.archetype.as_ref().unwrap();
        for (const Archetype& possible : m_world->m_archetypes) {
            if (possible.id() == archetype_id) {
                return possible.get<T>(m_entity);
            }
        }
    }
    return nullptr;
}

template<typename T>
EntityMut& EntityMut::insert(T&& component) {
    auto& entity = m_world->m_entites.get_mut(m_entity).unwrap();

    auto new_archetype_id = ArchetypeId();
    if (entity.archetype) {
        new_archetype_id = entity.archetype.as_ref().unwrap();
    }
    new_archetype_id.with(ComponentId::of<T>());

    // Find or create an archetype with archetype_id
    Archetype* dst_archetype = nullptr;
    for (Archetype& possible : m_world->m_archetypes) {
        if (possible.id() == new_archetype_id) {
            dst_archetype = &possible;
            break;
        }
    }
    if (!dst_archetype) {
        m_world->m_archetypes.push(Archetype(new_archetype_id));
        dst_archetype = &m_world->m_archetypes.last_mut().unwrap();
        dst_archetype->spawn(m_entity);
    }

    dst_archetype->insert(m_entity, core::forward<T>(component));

    // If the entity was already owned by an arechetype transfer it to a new one
    if (entity.archetype) {
        const auto old_archetype_id = entity.archetype.as_ref().unwrap();
        for (Archetype& possible : m_world->m_archetypes) {
            if (possible.id() == old_archetype_id) {
                possible.transfer(m_entity, *dst_archetype);
                break;
            }
        }
    }

    entity.archetype = new_archetype_id;
    return *this;
}

template<typename T>
Option<const T&> EntityMut::get() const {
    auto& entity = m_world->m_entites.get(m_entity).unwrap();
    if (entity.archetype) {
        const auto archetype_id = entity.archetype.as_ref().unwrap();
        for (const Archetype& possible : m_world->m_archetypes) {
            if (possible.id() == archetype_id) {
                return possible.get<T>(m_entity);
            }
        }
    }
    return nullptr;
}

template<typename T>
Option<T&> EntityMut::get_mut() const {
    auto& entity = m_world->m_entites.get_mut(m_entity).unwrap();
    if (entity.archetype) {
        const auto archetype_id = entity.archetype.as_ref().unwrap();
        for (Archetype& possible : m_world->m_archetypes) {
            if (possible.id() == archetype_id) {
                return possible.get_mut<T>(m_entity);
            }
        }
    }
    return nullptr;
}

GAME_NAMESPACE_END
