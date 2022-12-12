// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <usize count, typename T>
usize BucketArray<count, T>::len() const {
	const auto assumed = (m_buckets.len() - 1) * count;
	auto last = m_buckets.last();
	if (last.is_set()) {
		auto& real = last.unwrap();
		return assumed + real.len();
	}
	return assumed;
}

template <usize count, typename T>
usize BucketArray<count, T>::cap() const {
	return m_buckets.len() * count;
}

template <usize count, typename T>
T& BucketArray<count, T>::operator[](usize index) {
	const auto bucket = index / count;
	const auto remainder = index & count;
	return m_buckets[bucket].values[remainder];
}

template <usize count, typename T>
const T& BucketArray<count, T>::operator[](usize index) const {
	const auto bucket = index / count;
	const auto remainder = index & count;
	return m_buckets[bucket].values[remainder];
}

template <usize count, typename T>
void BucketArray<count, T>::reserve(usize amount) {
	m_buckets.reserve(amount);
}

template <usize count, typename T>
usize BucketArray<count, T>::push(T&& item) {
	const auto assumed = (m_buckets.len() - 1) * count;

	auto last = m_buckets.last_mut();
	if (last.is_set()) {
		auto& real = last.unwrap();
		const auto len = real.len();

		if (len < count) {
			real.push(core::forward<T>(item));
			return assumed + len;
		}
	}

	Array<T> values;
	values.push(core::forward<T>(item));
	m_buckets.push(Bucket {
		core::move(values),
	});

	return 0;
}

template <usize count, typename T>
Option<T> BucketArray<count, T>::pop() {
	const auto assumed = (m_buckets.len() - 1) * count;

	auto last = m_buckets.last_mut();
	if (last.is_set()) {
		auto& real = last.unwrap();
		auto value = real.pop();

		if (real.is_empty()) {
			m_buckets.pop();
		}

		return value;
	}

	return nullptr;
}

CORE_NAMESPACE_END
