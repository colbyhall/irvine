// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
const Quat<T> Quat<T>::identity = {};

template <typename T>
Quat<T> Quat<T>::from_axis_angle(Vec3<T> axis, T theta) {
	const auto half_theta = theta / (T)2;
	const auto s = core::sin(half_theta);
	const auto c = core::cos(half_theta);
	return Quat{ s * axis.x, s * axis.y, s * axis.z, c };
}

template <typename T>
Quat<T> Quat<T>::from_euler(T pitch, T yaw, T roll) {
	const auto rads_div_by_2 = (T)core::deg_to_rad / (T)2;

	const auto pitch1 = core::fmod(pitch, (T)360);
	const auto yaw1 = core::fmod(yaw, (T)360);
	const auto roll1 = core::fmod(roll, (T)360);

	const auto sp = core::sin(pitch1 * rads_div_by_2);
	const auto cp = core::cos(pitch1 * rads_div_by_2);

	const auto sy = core::sin(yaw1 * rads_div_by_2);
	const auto cy = core::cos(yaw1 * rads_div_by_2);

	const auto sr = core::sin(roll1 * rads_div_by_2);
	const auto cr = core::cos(roll1 * rads_div_by_2);

	return Quat{
		cr * sp * sy - sr * cp * cy,
		-cr * sp * cy - sr * cp * sy,
		cr * cp * sy - sr * sp * cy,
		cr * cp * cy + sr * sp * sy
	};
}

template <typename T>
Option<Quat<T>> Quat<T>::normalized() const {
	if (is_near_zero()) return Option<Quat>{};
	const auto inverse = (T)1 / len();
	return Quat{
		i * inverse,
		j * inverse,
		k * inverse,
		w * inverse
	};
}

template <typename T>
Vec3<T> Quat<T>::rotate_vector(const Vec3<T>& xyz) const {
	const Vec3<T> this_xyz = { i, j, k };
	const auto t = this_xyz.cross(xyz) * (T)2;
	return xyz + (t * w) + this_xyz.cross(t);
}

template <typename T>
Quat<T> Quat<T>::operator*(const Quat<T>& rhs) const {
	const auto result = Quat<T> {
		(((w * rhs.i) + (i * rhs.w)) + (j * rhs.k)) - (k * rhs.j),
		(((w * rhs.j) + (j * rhs.w)) + (k * rhs.i)) - (i * rhs.k),
		(((w * rhs.k) + (k * rhs.w)) + (i * rhs.j)) - (j * rhs.i),
		(((w * rhs.w) - (i * rhs.i)) - (j * rhs.j)) - (k * rhs.k)
	};
	return result.normalize().unwrap();
}

CORE_NAMESPACE_END
