// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
FORCE_INLINE Array<T>::Array(Slice<const T> slice) : m_ptr(nullptr), m_len(slice.len()), m_cap(0) {
	reserve(slice.len());
	for (u32 i = 0; i < slice.len(); ++i) {
		T copy = slice[i];
		new (m_ptr + i) T(core::move(copy));
	}
}

template <typename T>
FORCE_INLINE Array<T>::Array(Array<T>&& move) noexcept
	: m_ptr(move.m_ptr), m_len(move.m_len), m_cap(move.m_cap) {
	move.m_ptr = nullptr;
	move.m_len = 0;
	move.m_cap = 0;
}

template <typename T>
FORCE_INLINE Array<T>& Array<T>::operator=(Array<T>&& move) noexcept {
	Array<T> to_destroy = core::move(*this);
	m_ptr = move.m_ptr;
	m_len = move.m_len;
	m_cap = move.m_cap;
	move.m_ptr = nullptr;
	move.m_len = 0;
	move.m_cap = 0;
	return *this;
}

template <typename T>
Array<T>::~Array() {
	if (m_ptr) {
		for (usize i = 0; i < m_len; ++i) {
			T& item = m_ptr[i];
			item.~T();
		}
		mem::free(m_ptr);
		m_ptr = nullptr;
	}
}

template <typename T>
FORCE_INLINE T& Array<T>::operator[](usize index) {
	ASSERT(is_valid_index(index), "Index out of bounds");
	return m_ptr[index];
}

template <typename T>
FORCE_INLINE const T& Array<T>::operator[](usize index) const {
	ASSERT(is_valid_index(index), "Index out of bounds");
	return m_ptr[index];
}

template <typename T>
FORCE_INLINE Option<T&> Array<T>::last_mut() {
	if (len() > 0) return m_ptr[len() - 1];
	return nullptr;
}

template <typename T>
FORCE_INLINE Option<T const&> Array<T>::last() const {
	if (len() > 0) return m_ptr[len() - 1];
	return nullptr;
}

template <typename T>
FORCE_INLINE void Array<T>::set_len(usize len) {
	ASSERT(len <= m_cap);
	// FIXME: Delete the items that are lost and initialize any gained to default
	m_len = len;
}

template <typename T>
void Array<T>::reserve(usize amount) {
	// FIXME: Find proper way of allocating in larger blocks to prevent allocation per push
	const auto old_cap = m_cap;
	auto new_cap = old_cap + amount;
	while (m_cap < new_cap) {
		m_cap += new_cap >> 1;
		m_cap += 1;
	}

	// FIXME: Custom allocator support
	if (m_ptr == nullptr) {
		void* ptr = mem::alloc(mem::Layout::array<T>(m_cap));
		m_ptr = static_cast<T*>(ptr);
	}
	else {
		void* ptr = mem::realloc(m_ptr, mem::Layout::array<T>(old_cap), mem::Layout::array<T>(m_cap));
		m_ptr = static_cast<T*>(ptr);
	}
}

template <typename T>
void Array<T>::insert(usize index, T&& item) {
	ASSERT(index <= m_len);
	if (len() == cap()) reserve(1);

	auto* src = m_ptr + index;
	if (index != len()) {
		mem::move(src + 1, src, (len() - index) * sizeof(T));
		mem::set(src, 0, sizeof(T));
	}

	new (src) T(core::forward<T>(item));
	m_len += 1;
}

template <typename T>
FORCE_INLINE void Array<T>::insert(usize index, const T& item) {
	T copy = item;
	insert(index, core::move(copy));
}

template <typename T>
T Array<T>::remove(usize index) {
	ASSERT(index < m_len);

	T result = core::move(m_ptr[index]);
	void* clear = &m_ptr[index];
	mem::set(clear, 0, sizeof(T));
	if (index < m_len - 1) {
		auto* src = m_ptr + index;
		mem::move(src, src + 1, (len() - index) * sizeof(T));
	}
	m_len -= 1;
	return result;
}

template <typename T>
FORCE_INLINE usize Array<T>::push(T&& item) {
	const auto index = len();
	insert(index, core::move(item));
	return index;
}

template <typename T>
FORCE_INLINE usize Array<T>::push(const T& item) {
	T copy = item;
	return push(core::move(copy));
}

template <typename T>
FORCE_INLINE Option<T> Array<T>::pop() {
	if (m_len > 0) {
		m_len -= 1;
		return core::move(m_ptr[m_len]);
	}
	return nullptr;
} 

CORE_NAMESPACE_END
