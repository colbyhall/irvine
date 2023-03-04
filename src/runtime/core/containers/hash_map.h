// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.h>
#include <core/non_copyable.h>
#include <core/containers/array.h>
#include <core/containers/function.h>
#include <core/templates/type_traits.h>

CORE_NAMESPACE_BEGIN

template <typename Key, typename Value, typename Hasher>
class HashMap;

template <typename Key, typename Value, typename Hasher>
class HashMapIterator {
public:
	inline explicit HashMapIterator(HashMap<Key, Value, Hasher>& hash_map)
		: m_hash_map(hash_map) {}

	inline explicit operator bool() const { return m_index < m_hash_map.m_buckets.len(); }
	inline HashMapIterator& operator++() { m_index += 1; return *this; }
	inline const Key& key() const { return m_hash_map.m_buckets[m_index].key; }
	inline Value& value() const { return m_hash_map.m_buckets[m_index].value; }

private:
	usize m_index = 0;
	HashMap<Key, Value, Hasher>& m_hash_map;
};

template <typename Key, typename Value, typename Hasher>
class ConstHashMapIterator {
public:
	inline explicit ConstHashMapIterator(const HashMap<Key, Value, Hasher>& hash_map)
		: m_hash_map(hash_map) {}

	inline explicit operator bool() const { return m_index < m_hash_map.m_buckets.len(); }
	inline ConstHashMapIterator& operator++() { m_index += 1; return *this; }
	inline const Key& key() const { return m_hash_map.m_buckets[m_index].key; }
	inline const Value& value() const { return m_hash_map.m_buckets[m_index].value; }

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
	static_assert(
		core::is_base_of<core::Hasher, Hasher>,
		"Hasher must be a valid Hasher"
	);

public:
	constexpr HashMap() = default;

	using ConstIterator = ConstHashMapIterator<Key, Value, Hasher>;
	using Iterator = HashMapIterator<Key, Value, Hasher>;

	void reserve(usize amount);
	void insert(const Key& key, Value&& value);
	void insert(const Key& key, const Value& value);
	Option<Value> remove(const Key& key);

	void retain(FunctionRef<bool(const Key&, const Value&)> keep);

	Option<Value const&> find(const Key& key) const;
	Option<Value&> find_mut(const Key& key);

	inline ConstIterator iter() const;
	inline Iterator iter_mut();

private:
	friend class HashMapIterator<Key, Value, Hasher>;
	friend class ConstHashMapIterator<Key, Value, Hasher>;

	inline usize key_to_layout_index(const Key& key) const;
	void refresh_layout();

	Array<Bucket> m_buckets;
	Array<i32> m_layout;
};

CORE_NAMESPACE_END

#include <core/containers/hash_map.inl>

using core::HashMap;
