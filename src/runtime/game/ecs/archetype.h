// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <game/ecs/entity.h>
#include <game/ecs/storage.h>

#include <core/non_copyable.h>
#include <core/containers/bucket_array.h>
#include <core/containers/hash_map.h>
#include <core/containers/option.h>
#include <core/containers/unique.h>
#include <core/templates/type_traits.h>


GAME_NAMESPACE_BEGIN

class Archetype {
public:
    explicit Archetype(const ArchetypeId& id) : m_id(id) {}
    
    void spawn(EntityId entity);
    void transfer(EntityId entity, Archetype& dst);

    template <typename T>
    void insert(EntityId entity, T&& component) {
        static_assert(core::is_base_of<Component, T>, "T must be derived from Component");

        auto lookup = m_lookup.find(entity);
        ASSERT(lookup.is_set(), "entity must be owned by archetype to insert a component");
        const u32 entity_lookup = lookup.unwrap();

        const auto id = ComponentId::of<T>();
        ASSERT(m_id.contains(id), "Archetype must be setup to support component of T");

        Option<Unique<Storage>&> possible_storage = m_components.find_mut(id);
        if (possible_storage) {
            auto& storage = possible_storage.unwrap();
            auto& bucket_storage = static_cast<BucketStorage<T>&>(*storage);
            bucket_storage.insert(entity_lookup, core::forward<T>(component));
        }
        else {
            auto storage = make_unique<BucketStorage<T>>();
            storage->insert(entity_lookup, core::forward<T>(component));
            m_components.insert(id, core::move(storage));
        }
    }

    template <typename T>
    Option<const T&> get(EntityId entity) const {
        static_assert(core::is_base_of<Component, T>, "T must be derived from Component");

        auto lookup = m_lookup.find(entity);
        ASSERT(lookup.is_set(), "entity must be owned by archetype to insert a component");
        const u32 entity_lookup = lookup.unwrap();

        const auto id = ComponentId::of<T>();
        ASSERT(m_id.contains(id), "Archetype must be setup to support component of T");

        Option<const Unique<Storage>&> possible_storage = m_components.find(id);
        if (possible_storage) {
            auto& storage = possible_storage.unwrap();
            auto component = storage->get(entity_lookup);
            if (component) {
                return static_cast<const T&>(component.unwrap());
            }
        }

        return nullptr;
    }

    template <typename T>
    Option<T&> get_mut(EntityId entity) {
        static_assert(core::is_base_of<Component, T>, "T must be derived from Component");

        auto lookup = m_lookup.find(entity);
        ASSERT(lookup.is_set(), "entity must be owned by archetype to insert a component");
        const u32 entity_lookup = lookup.unwrap();

        const auto id = ComponentId::of<T>();
        ASSERT(m_id.contains(id), "Archetype must be setup to support component of T");

        Option<Unique<Storage>&> possible_storage = m_components.find_mut(id);
        if (possible_storage) {
            auto& storage = possible_storage.unwrap();
            auto component = storage->get_mut(entity_lookup);
            if (component) {
                return static_cast<T&>(component.unwrap());
            }
        }

        return nullptr;
    }

    inline ArchetypeId id() const { return m_id; }

private:
    ArchetypeId m_id;

    BucketArray<game::storage_bucket_size, Option<EntityId>> m_entities;
    HashMap<ComponentId, Unique<Storage>> m_components;
    Array<u32> m_free_list;
    HashMap<EntityId, u32> m_lookup;
};

GAME_NAMESPACE_END
