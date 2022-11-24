// Copyright Colby Hall. All Rights Reserved.

template <typename T>
INLINE constexpr NonNull<T>::NonNull(T* ptr) : m_ptr(ptr) {
	ASSERT(ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
}

template <typename T>
INLINE NonNull<T>::operator T* () const {
	return m_ptr;
}

template <typename T>
INLINE NonNull<T>::operator void* () const {
	return m_ptr;
}

template <typename T>
INLINE T* NonNull<T>::operator->() const {
	return m_ptr;
}

template <typename T>
INLINE T& NonNull<T>::operator*() const {
	return *m_ptr;
}

INLINE constexpr NonNull<void>::NonNull(void* ptr) : m_ptr(ptr) {
	ASSERT(ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
}

INLINE NonNull<void>::operator void* () const {
	return m_ptr;
}

INLINE constexpr NonNull<void const>::NonNull(void const* ptr) : m_ptr(ptr) {
	ASSERT(ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
}

INLINE NonNull<void const>::operator void const* () const {
	return m_ptr;
}
