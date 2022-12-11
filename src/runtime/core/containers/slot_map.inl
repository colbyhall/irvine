// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
void SlotMap<T>::reserve(usize amount) {
	m_generations.reserve(amount);
	m_values.reserve(amount);
	m_free_list.reserve(amount);
}

template <typename T>
SlotKey SlotMap<T>::insert(T&& t) {
	// If we have an item in the free list then reuse the slot
	const auto possible_index = m_free_list.pop();
	if (possible_index) {
		const auto index = possible_index.unwrap();
		ASSERT(!m_values[index].is_set(), "Free list indies must point to null value");

		m_generations[index] += 1;
		m_values[index] = core::forward<T>(t);

		return SlotKey{
			.generation = m_generations[value],
			.index = index
		};
	}

	// Create a new slot for the value
	m_generations.push(0);
	m_values.push(core::forward<T>(t));

	return SlotKey{
		.generation = 0,
		.index = (u32)(m_values.len() - 1),
	};
}

template <typename T>
bool SlotMap<T>::remove(SlotKey key) {
	const bool can_remove = m_values.is_valid_index(key.index) &&
		m_generations[key.index] == key.index &&
		m_values[key.index].is_set();
	if (can_remove) {
		m_values[key.index] = nullptr;
		return true;
	}
	return false;
}

template <typename T>
inline bool SlotMap<T>::contains(SlotKey key) const {
	return m_values.is_valid_index(key.index) &&
		m_generations[key.index] == key.index &&
		m_values[key.index].is_set();
}

template <typename T>
inline Option<const T&> SlotMap<T>::get(SlotKey key) const {
	if (contains(key)) {
		return m_values[key.index].as_ref();
	}
	return nullptr;
}

template <typename T>
inline Option<T&> SlotMap<T>::get_mut(SlotKey key) {
	if (contains(key)) {
		return m_values[key.index].as_mut();
	}
	return nullptr;
}

CORE_NAMESPACE_END
