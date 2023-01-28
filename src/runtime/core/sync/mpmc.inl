// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
MPMCQueue<T> MPMCQueue<T>::make(usize size) {
	// Verify that size is a power of 2
	ASSERT(size >= 2 && (size & size - 1) == 0);

	const auto layout = mem::Layout::array<Cell>(size);
	void* memory = mem::alloc(layout);
	mem::set(memory, 0, layout.size);
	Cell* buffer = (Cell*)memory;
	for (usize i = 0; i < size; ++i) {
		Cell& cell = buffer[i];
		cell.sequence.store((int)i, Order::Relaxed);
	}

	return MPMCQueue(buffer, (int)size);
}

template <typename T>
FORCE_INLINE MPMCQueue<T>::MPMCQueue(MPMCQueue<T>&& move) noexcept :
	m_buffer(move.m_buffer),
	m_buffer_mask(move.m_buffer_mask),
	m_enqueue_pos(move.m_enqueue_pos.load(Order::Relaxed)),
	m_dequeue_pos(move.m_dequeue_pos.load(Order::Relaxed)) {
	move.m_buffer = nullptr;
	move.m_buffer_mask = 0;
}

template <typename T>
FORCE_INLINE MPMCQueue<T>& MPMCQueue<T>::operator=(MPMCQueue<T>&& move) noexcept {
	// FIXME: Is this the best way to do this
	MPMCQueue<T> to_destroy = core::move(*this);
	m_buffer = move.m_ptr;
	m_buffer_mask = move.m_len;
	const auto enqueue_pos = move.m_enqueue_pos.load(Order::Relaxed);
	m_enqueue_pos.store(enqueue_pos, Order::Relaxed);
	const auto dequeue_pos = move.m_dequeue_pos.load(Order::Relaxed);
	m_dequeue_pos.store(dequeue_pos, Order::Relaxed);

	move.m_buffer = nullptr;
	move.m_buffer_mask = 0;
	return *this;
}

template <typename T>
MPMCQueue<T>::~MPMCQueue() {
	if (m_buffer) {
		// FIXME: Free elements
		mem::free(m_buffer);
	}
}

template <typename T>
bool MPMCQueue<T>::push(T&& t) const {
	Cell* cell = nullptr;
	auto pos = m_enqueue_pos.load();
	while (true) {
		cell = &m_buffer[pos & m_buffer_mask];
		const auto seq = cell->sequence.load();
		const auto dif = seq - pos;

		if (dif == 0) {
			if (!m_enqueue_pos.compare_exchange_weak(pos, pos + 1).is_set())
				break;
		} else if (dif < 0) return false;
		else pos = m_enqueue_pos.load();
	}
	cell->data = core::forward<T>(t);
	cell->sequence.store(pos + 1);
	return true;
}

template <typename T>
FORCE_INLINE bool MPMCQueue<T>::push(const T& t) const {
	T copy = t;
	return push(core::move(copy));
}

template <typename T>
Option<T> MPMCQueue<T>::pop() const {
	Cell* cell = nullptr;
	auto pos = m_dequeue_pos.load();
	while (true) {
		cell = &m_buffer[pos & m_buffer_mask];
		const auto seq = cell->sequence.load();
		const auto dif = seq - (pos + 1);

		if (dif == 0) {
			if (!m_dequeue_pos.compare_exchange_weak(pos, pos + 1).is_set())
				break;
		} else if (dif < 0) return nullptr;
		else pos = m_dequeue_pos.load();
	}
	T t = cell->data.unwrap();
	cell->sequence.store(pos + m_buffer_mask + 1);
	return t;
}

CORE_NAMESPACE_END
