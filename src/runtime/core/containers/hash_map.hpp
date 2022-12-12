// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.hpp>
#include <core/non_copyable.hpp>
#include <core/containers/array.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

template <typename Key, typename Value, typename Hasher = FNV1Hasher>
class HashMap {
    struct Bucket {
        Key key;
        Value value;
        i32 next;
    };
    static_assert(core::is_base_of<core::Hasher, Hasher>, "Hasher must be a valid Hasher");

public:
    constexpr HashMap() = default;

    void reserve(usize amount);
    void insert(Key&& key, Value&& value);
    void insert(const Key& key, const Value& value);

    Option<Value const&> find(const Key& key) const;
    Option<Value&> find_mut(const Key& key);

private:
    FORCE_INLINE usize key_to_layout_index(const Key& key) const;
    void refresh_layout();

    Array<Bucket> m_buckets;
    Array<i32> m_layout;
};

CORE_NAMESPACE_END

#include <core/containers/hash_map.inl>

using core::HashMap;