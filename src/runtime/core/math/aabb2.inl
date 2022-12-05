// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
constexpr AABB2<T> AABB2<T>::from_center(const Vec2<T>& location, const Vec2<T>& half_size) {
	return AABB2<T> { location, half_size };
}

template <typename T>
constexpr AABB2<T> AABB2<T>::from_min_max(const Vec2<T>& min, const Vec2<T>& max) {
	const auto half_size = (max - min) / 2;
	const auto position = min + half_size;
	return AABB2<T> { position, half_size };
}

template <typename T>
FORCE_INLINE Vec2<T> AABB2<T>::bottom_right() const {
	auto result = m_position;
	result.x += m_half_size.x;
	result.y -= m_half_size.y;
	return result;
}

template <typename T>
FORCE_INLINE Vec2<T> AABB2<T>::top_left() const {
	auto result = m_position;
	result.x -= m_half_size.x;
	result.y += m_half_size.y;
	return result;
}

CORE_NAMESPACE_END