// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename Key, typename Value, typename Hasher>
void HashMap<Key, Value, Hasher>::reserve(usize amount) {
	m_buckets.reserve(amount);
	m_layout.reserve(amount);
}

template <typename Key, typename Value, typename Hasher>
void HashMap<Key, Value, Hasher>::insert(const Key& key, Value&& value) {
	m_buckets.push(Bucket {
		key,
		core::forward<Value>(value),
		-1
	});

	refresh_layout();
}

template <typename Key, typename Value, typename Hasher>
void HashMap<Key, Value, Hasher>::insert(const Key& key, const Value& value) {
	m_buckets.push(Bucket {
		key,
		value,
		-1
	});

	refresh_layout();
}

template <typename Key, typename Value, typename Hasher>
Option<Value> HashMap<Key, Value, Hasher>::remove(const Key& key) {
	if (m_buckets.is_empty()) {
		return nullptr;
	}

	const auto index = key_to_layout_index(key);
	const auto mapped = m_layout[index];
	if (mapped == -1) {
		return nullptr;
	}

	Bucket* bucket = &m_buckets[mapped];
	i32 bucket_index = -1;
	while (true) {
		bucket_index = bucket->next;
		if (bucket->key == key) return bucket->value;
		if (bucket->next == -1) break;
		bucket = &m_buckets[bucket->next];
	}

	Bucket result = m_buckets.remove((usize)bucket_index);
	refresh_layout();
	return result.value;
}

template <typename Key, typename Value, typename Hasher>
Option<Value&> HashMap<Key, Value, Hasher>::find_mut(const Key& key) {
	if (m_buckets.is_empty()) {
		return nullptr;
	}

	const auto index = key_to_layout_index(key);
	const auto mapped = m_layout[index];
	if (mapped == -1) {
		return nullptr;
	}

	Bucket* bucket = &m_buckets[mapped];
	while (true) {
		if (bucket->key == key) return bucket->value;
		if (bucket->next == -1) break;
		bucket = &m_buckets[bucket->next];
	}

	return nullptr;
}

template <typename Key, typename Value, typename Hasher>
Option<Value const&> HashMap<Key, Value, Hasher>::find(const Key& key) const {
	if (m_buckets.is_empty()) {
		return nullptr;
	}

	const auto index = key_to_layout_index(key);
	const auto mapped = m_layout[index];
	if (mapped == -1) {
		return nullptr;
	}

	Bucket const* bucket = &m_buckets[mapped];
	while (true) {
		if (bucket->key == key) return bucket->value;
		if (bucket->next == -1) break;
		bucket = &m_buckets[bucket->next];
	}

	return nullptr;
}

template <typename Key, typename Value, typename Hasher>
FORCE_INLINE usize HashMap<Key, Value, Hasher>::key_to_layout_index(const Key& key) const {
	Hasher hasher = {};
	hash(hasher, key);
	const u64 the_hash = hasher.finish();
	return the_hash % m_buckets.len();
}

template <typename Key, typename Value, typename Hasher>
void HashMap<Key, Value, Hasher>::refresh_layout() {
	// Reset the layout array to all be invalid
	m_layout.set_len(0); // FIXME: Clear
	for (usize i = 0; i < m_buckets.len(); ++i) {
		m_layout.push(-1);
	}

	// Layout buckets by hash(key) & buckets.len() and build tree if collision detected
	for (usize i = 0; i < m_buckets.len(); ++i) {
		auto& bucket = m_buckets[i];

		const auto layout_index = key_to_layout_index(bucket.key);
		bucket.next = -1;

		// Check what index lies in the layout array
		auto found = m_layout[layout_index];

		// If its invalid then simply set the bucket index
		if (found == -1) {
			m_layout[layout_index] = (i32)i;
		}
		else {
			// If its valid then descend the bucket tree until an empty spot is found
			auto* other = &m_buckets[found];
			while (other->next != -1) other = &m_buckets[(usize)other->next];
			other->next = (i32)i;
		}
	}
}

template <typename Key, typename Value, typename Hasher>
FORCE_INLINE ConstHashMapIterator<Key, Value, Hasher> HashMap<Key, Value, Hasher>::iter() const {
	return ConstHashMapIterator(*this);
}


template <typename Key, typename Value, typename Hasher>
FORCE_INLINE HashMapIterator<Key, Value, Hasher> HashMap<Key, Value, Hasher>::iter_mut() {
	return HashMapIterator(*this);
}


CORE_NAMESPACE_END
