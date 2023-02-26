// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
void SlotMap<T>::reserve(usize amount) {
	m_generations.reserve(amount);
	m_values.reserve(amount);
}

template <typename T>
SlotKey SlotMap<T>::insert(T&& t) {
	const auto opt_index = m_free_list.pop();
	if (opt_index) {
		const auto index = opt_index.unwrap();
		m_values[index] = core::forward<T>(t);
		return SlotKey{
			.generation = m_generations[index],
			.index = index
		};
	}

	m_generations.push(0);
	m_values.push(core::forward<T>(t));
	return SlotKey{
		.generation = 0,
		.index = (u32)m_values.len() - 1
	};
}

template <typename T>
bool SlotMap<T>::remove(SlotKey key) {
	if (m_generations.is_valid_index(key.index)) {
		if (m_generations[key.index] == key.generation) {
			m_generations[key.index] += 1;
			m_values[key.index].unwrap();
			m_free_list.push(key.index);
			return true;
		}
	}

	return false;
}

template <typename T>
inline bool SlotMap<T>::contains(SlotKey key) const {
	if (m_generations.is_valid_index(key.index)) {
		if (m_generations[key.index] == key.generation) {
			return true;
		}
	}

	return false;
}

template <typename T>
inline Option<const T&> SlotMap<T>::get(SlotKey key) const {
	if (m_generations.is_valid_index(key.index)) {
		if (m_generations[key.index] == key.generation) {
			return m_values[key.index].as_ref();
		}
	}

	return nullptr;
}

template <typename T>
inline Option<T&> SlotMap<T>::get_mut(SlotKey key) {
	if (m_generations.is_valid_index(key.index)) {
		if (m_generations[key.index] == key.generation) {
			return m_values[key.index].as_mut();
		}
	}

	return nullptr;
}

CORE_NAMESPACE_END
