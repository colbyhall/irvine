// Copyright Colby Hall. All Rights Reserved.

template <typename T>
INLINE constexpr Slice<T>::Slice(T* ptr, usize len) : m_ptr(ptr), m_len(len) {
	ASSERT(ptr != nullptr || (ptr == nullptr && len == 0), "If we have no ptr we must not have len");
}

template <typename T>
INLINE usize Slice<T>::len() const {
	return m_len;
}

template <typename T>
INLINE bool Slice<T>::is_empty() const {
	return len() == 0;
}

template <typename T>
INLINE bool Slice<T>::is_valid_index(usize index) const {
	return index < len();
}

template <typename T>
INLINE T* Slice<T>::begin() {
	return m_ptr;
}

template <typename T>
INLINE T* Slice<T>::end() {
	return m_ptr + len();
}

template <typename T>
INLINE T const* Slice<T>::cbegin() const {
	return m_ptr;
}

template <typename T>
INLINE T const* Slice<T>::cend() const {
	return m_ptr + len();
}

template <typename T>
INLINE T& Slice<T>::operator[](usize index) {
	ASSERT(is_valid_index(index), "Index out of bounds.");
	return m_ptr[index];
}

template <typename T>
INLINE T const& Slice<T>::operator[](usize index) const {
	ASSERT(is_valid_index(index), "Index out of bounds.");
	return m_ptr[index];
}

template <typename T>
INLINE Slice<T>::operator bool() const {
	return !is_empty();
}

template <typename T>
INLINE Slice<T>::operator Slice<const T>() const {
	return Slice<const T>{ m_ptr, m_len };
}

template <typename T>
INLINE usize Slice<T>::shrink(usize amount) {
	ASSERT(amount <= len(), "Can not shrink more than len");
	m_len -= amount;
	return len();
}

template <typename T>
INLINE constexpr Slice<T const>::Slice(T const* ptr, usize len) : m_ptr(ptr), m_len(len) {
	ASSERT(ptr != nullptr || (ptr == nullptr && len == 0), "If we have no ptr we must not have len");
}

template <typename T>
INLINE usize Slice<T const>::len() const {
	return m_len;
}

template <typename T>
INLINE bool Slice<T const>::is_empty() const {
	return len() == 0;
}

template <typename T>
INLINE bool Slice<T const>::is_valid_index(usize index) const {
	return index < len();
}

template <typename T>
INLINE T const* Slice<T const>::cbegin() const {
	return m_ptr;
}

template <typename T>
INLINE T const* Slice<T const>::cend() const {
	return m_ptr + len();
}

template <typename T>
INLINE T const& Slice<T const>::operator[](usize index) const {
	ASSERT(is_valid_index(index), "Index out of bounds.");
	return m_ptr[index];
}

template <typename T>
INLINE Slice<T const>::operator bool() const {
	return !is_empty();
}

template <typename T>
INLINE usize Slice<T const>::shrink(usize amount) {
	ASSERT(amount <= len(), "Can not shrink more than len");
	m_len -= amount;
	return len();
}
