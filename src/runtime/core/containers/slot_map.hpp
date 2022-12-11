// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.hpp>
#include <core/containers/option.hpp>

CORE_NAMESPACE_BEGIN

struct SlotKey {
    u32 generation;
    u32 index;
};

template <typename T>
class SlotMap : private NonCopyable {
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

using core::SlotMap;
