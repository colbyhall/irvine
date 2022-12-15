// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.hpp>
#include <core/non_copyable.hpp>
#include <core/containers/array.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

template <typename Key, typename Value, typename Hasher>
class HashMap;

template <typename Key, typename Value, typename Hasher>
class HashMapIterator {
public:
    FORCE_INLINE explicit HashMapIterator(HashMap<Key, Value, Hasher>& hash_map) : m_hash_map(hash_map) {}

    FORCE_INLINE explicit operator bool() const { return m_index < m_hash_map.m_buckets.len(); }
    FORCE_INLINE HashMapIterator& operator++() { m_index += 1; return *this; }
    FORCE_INLINE const Key& key() const { return m_hash_map.m_buckets[m_index].key; }
    FORCE_INLINE Value& value() const { return m_hash_map.m_buckets[m_index].value; }

private:
    usize m_index = 0;
    HashMap<Key, Value, Hasher>& m_hash_map;
};

template <typename Key, typename Value, typename Hasher>
class ConstHashMapIterator {
public:
    FORCE_INLINE explicit ConstHashMapIterator(const HashMap<Key, Value, Hasher>& hash_map) : m_hash_map(hash_map) {}

    FORCE_INLINE explicit operator bool() const { return m_index < m_hash_map.m_buckets.len(); }
    FORCE_INLINE ConstHashMapIterator& operator++() { m_index += 1; return *this; }
    FORCE_INLINE const Key& key() const { return m_hash_map.m_buckets[m_index].key; }
    FORCE_INLINE const Value& value() const { return m_hash_map.m_buckets[m_index].value; }

private:
    usize m_index = 0;
    const HashMap<Key, Value, Hasher>& m_hash_map;
};

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

    using ConstIterator = ConstHashMapIterator<Key, Value, Hasher>;
    using Iterator = HashMapIterator<Key, Value, Hasher>;

    void reserve(usize amount);
    void insert(const Key& key, Value&& value);
    void insert(const Key& key, const Value& value);
    Option<Value> remove(const Key& key);

    Option<Value const&> find(const Key& key) const;
    Option<Value&> find_mut(const Key& key);

    FORCE_INLINE ConstIterator iter() const;
    FORCE_INLINE Iterator iter_mut();

private:
    friend class HashMapIterator<Key, Value, Hasher>;
    friend class ConstHashMapIterator<Key, Value, Hasher>;

    FORCE_INLINE usize key_to_layout_index(const Key& key) const;
    void refresh_layout();

    Array<Bucket> m_buckets;
    Array<i32> m_layout;
};

CORE_NAMESPACE_END

#include <core/containers/hash_map.inl>

using core::HashMap;