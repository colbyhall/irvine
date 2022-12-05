// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T, SMode Mode, typename... Args>
FORCE_INLINE Shared<T, Mode> make_shared(Args&&... args) {
	constexpr auto counter_layout = mem::Layout::single<SharedCounter<Mode>>;
	auto layout = mem::Layout::single<T>;
	layout.alignment = core::max(layout.alignment, counter_layout.alignment);
	layout.size += counter_layout.size;

	void* ptr = mem::alloc(layout);
	void* result = new (ptr) SharedCounter<Mode>(1, 0);
	ptr = &((SharedCounter<Mode>*)ptr)[1];
	new (ptr) T(forward<Args>(args)...);

    return Shared<T, Mode>(result);
}

template <typename Base, SMode Mode>
Shared<Base, Mode>::~Shared() {
	if (m_ptr) {
		auto& c = counter();

		// Decrement the strong count
		const auto strong_count = c.remove_strong();
		const auto weak_count = c.weak();

		// If there are no strong references deconstruct the object
		if (strong_count == 0) {
			value().~Base();

			// Free the memory if we have no weak references
			if (weak_count == 0) {
				mem::free(m_ptr);
			}

			m_ptr = nullptr;
		}
	}
}

template <typename Base, SMode Mode>
Weak<Base, Mode> Shared<Base, Mode>::downgrade() const {
	auto& c = counter();
	c.add_weak();
	return Weak<Base, Mode>(m_ptr);
}

template <typename Base, SMode Mode>
Shared<Base, Mode> Shared<Base, Mode>::clone() const {
	auto& c = counter();
	c.add_strong();
	return Shared<Base, Mode>(m_ptr);
}

template <typename Base, SMode Mode>
Weak<Base, Mode>::~Weak() {
	if (m_ptr) {
		auto& c = counter();

		// Decrement the weak count
		const auto strong_count = c.strong();
		const auto weak_count = c.remove_weak();

		// If there are no strong and weak references free the memory
		if (strong_count == 0 && weak_count == 0) {
			mem::free(m_ptr);
			m_ptr = nullptr;
		}
	}
}

template <typename Base, SMode Mode>
Option<Shared<Base, Mode>> Weak<Base, Mode>::upgrade() const {
	auto& c = counter();
	const auto strong_count = c.strong();
	if (strong_count > 0) {
		c.add_strong();
		return Shared<Base, Mode>(m_ptr);
	}
	return nullptr;
}

template <typename Base, SMode Mode>
Weak<Base, Mode> Weak<Base, Mode>::clone() const {
	auto& c = counter();
	c.add_weak();
	return Weak<Base, Mode>(m_ptr);
}

CORE_NAMESPACE_END
