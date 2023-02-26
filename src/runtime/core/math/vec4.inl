// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
inline T Vec4<T>::dot(const Vec4<T>& rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

template <typename T>
inline Option<Vec4<T>> Vec4<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

CORE_NAMESPACE_END
