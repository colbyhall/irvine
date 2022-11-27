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
		const auto ref_count = c.remove_strong();
		if (ref_count == 0) {
			value().~Base();
			mem::free(m_ptr);
			m_ptr = nullptr;
		}
	}
}

template <typename Base, SMode Mode>
Shared<Base, Mode> Shared<Base, Mode>::clone() const {
	auto& c = counter();
	c.add_strong();
	return Shared<Base, Mode>(m_ptr);
}

CORE_NAMESPACE_END
