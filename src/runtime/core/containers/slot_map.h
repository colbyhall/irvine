// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.h>
#include <core/containers/array.h>
#include <core/containers/option.h>

CORE_NAMESPACE_BEGIN

struct SlotKey {
    u32 generation;
    u32 index;

    inline bool operator==(const SlotKey& rhs) const {
        return generation == rhs.generation && index == rhs.index;
    }
};

template <typename T>
class SlotMap {
public:
    SlotMap() = default;

    void reserve(usize amount);
    SlotKey insert(T&& t);
    bool remove(SlotKey key);

    inline bool contains(SlotKey key) const;
    inline Option<const T&> get(SlotKey key) const;
    inline Option<T&> get_mut(SlotKey key);

private:
    Array<u32> m_generations;
    Array<Option<T>> m_values;
    Array<u32> m_free_list;
};

CORE_NAMESPACE_END

#include <core/containers/slot_map.inl>

using core::SlotKey;
using core::SlotMap;

SUPPRESS_WARNING_PUSH
SUPPRESS_WARNINGS

inline void hash(Hasher& hasher, const SlotKey& key) {
    hash(hasher, key.generation);
    hash(hasher, key.index);
}

SUPPRESS_WARNING_POP
