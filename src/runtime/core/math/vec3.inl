// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
const Vec3<T> Vec3<T>::zero = { 0 };

template <typename T>
const Vec3<T> Vec3<T>::one = { 1 };

template <typename T>
const Vec3<T> Vec3<T>::infinite = { core::infinity };

template <typename T>
const Vec3<T> Vec3<T>::forward = { 1, 0, 0 };

template <typename T>
const Vec3<T> Vec3<T>::right = { 0, 1, 0 };

template <typename T>
const Vec3<T> Vec3<T>::up = { 0, 0, 1 };

template <typename T>
void Vec3<T>::orthonormal_basis(Vec3<T>& f, Vec3<T>& r, Vec3<T>& u) {
	// Copied from unreal
	const auto DELTA = (T)0.00001;

	// Project the X and Y axes onto the plane perpendicular to the Z axis.
	f -= u * (f.dot(u) / u.len_sq());
	r -= u * (r.dot(u) / u.len_sq());

	// If the X axis was parallel to the Z axis, choose a vector which is orthogonal to the Y and Z axes.
	if (f.len_sq() < DELTA * DELTA) {
		f = r.cross(u);
	}

	// If the Y axis was parallel to the Z axis, choose a vector which is orthogonal to the X and Z axes.
	if (r.len_sq() < DELTA * DELTA) {
		r = f.cross(r);
	}
}

template <typename T>
FORCE_INLINE Vec3<T> Vec3<T>::cross(const Vec3<T>& rhs) const {
	return {
		y * rhs.z - rhs.y * z,
		z * rhs.x - rhs.z * x,
		x * rhs.y - rhs.x * y
	};
}

template <typename T>
FORCE_INLINE Option<Vec3<T>> Vec3<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

CORE_NAMESPACE_END
