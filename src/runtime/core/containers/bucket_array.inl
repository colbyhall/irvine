// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <usize size, typename T>
usize BucketArray<size, T>::len() const {
	usize result = 0;
	for (usize i = 0; i < m_buckets.len(); ++i) {
		result += m_buckets[i].values.len();
	}
	return result;
}

template <usize size, typename T>
usize BucketArray<size, T>::cap() const {
	return m_buckets.len() * size;
}

template <usize size, typename T>
T& BucketArray<size, T>::operator[](usize index) {
	const auto bucket = index / size;
	const auto remainder = index & size;
	return m_buckets[bucket].values[remainder];
}

template <usize size, typename T>
const T& BucketArray<size, T>::operator[](usize index) const {
	const auto bucket = index / size;
	const auto remainder = index & size;
	return m_buckets[bucket].values[remainder];
}

template <usize size, typename T>
void BucketArray<size, T>::reserve(usize amount) {
	m_buckets.reserve(amount);
}

template <usize size, typename T>
usize BucketArray<size, T>::push(T&& item) {
	auto last = m_buckets.last_mut();
	if (last) {
		auto& real = last.unwrap();
		const auto len = real.values.len();

		if (len < size) {
			real.values.push(core::forward<T>(item));
			return m_buckets.len() * size + len;
		}
	}

	Array<T> values;
	values.reserve(size);
	values.push(core::forward<T>(item));
	m_buckets.push(Bucket {
		core::move(values),
	});

	return 0;
}

template <usize size, typename T>
Option<T> BucketArray<size, T>::pop() {
	auto last = m_buckets.last_mut();
	if (last.is_set()) {
		auto& real = last.unwrap();
		auto value = real.values.pop();

		if (real.is_empty()) {
			m_buckets.pop();
		}

		return value;
	}

	return nullptr;
}

CORE_NAMESPACE_END
