// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
FORCE_INLINE Vec2<T> Vec2<T>::from_rad(T theta) {
	return { core::sin(theta), core::cos(theta) };
}

template <typename T>
const Vec2<T> Vec2<T>::zero = { 0 };

template <typename T>
const Vec2<T> Vec2<T>::one = { 1 };

template <typename T>
const Vec2<T> Vec2<T>::infinite = { core::infinity };

template <typename T>
const Vec2<T> Vec2<T>::right = { 1, 0 };

template <typename T>
const Vec2<T> Vec2<T>::up = { 0, 1 };

template <typename T>
FORCE_INLINE Option<Vec2<T>> Vec2<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

CORE_NAMESPACE_END
