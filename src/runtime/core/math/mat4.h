// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/quat.h>
#include <core/math/vec4.h>

CORE_NAMESPACE_BEGIN

template <typename T>
struct Mat4 {
	union {
		struct { Vec4<T> x, y, z, w; };
		Vec4<T> columns[4];
		T elements[4 * 4];
	};

	inline constexpr Mat4() : x(0), y(0), z(0), w(0) {}

	static constexpr Mat4 from_columns(Vec4<T> x, Vec4<T> y, Vec4<T> z, Vec4<T> w);
	static constexpr Mat4 from_rows(Vec4<T> x, Vec4<T> y, Vec4<T> z, Vec4<T> w);
	static constexpr Mat4 orthographic(T width, T height, T near, T far);
	static Mat4 perspective(T fov, T aspect_ratio, T near, T far);
	static Mat4 transform(Vec3<T> position, Quat<T> rotation, Vec3<T> scale);

	static const Mat4 identity;

	Option<Mat4> inverse() const;

	Vec4<T> row(usize index) const;

	Mat4 operator*(const Mat4& rhs) const;
	Vec4<T> operator*(const Vec4<T>& rhs) const;

private:
	inline constexpr Mat4(Vec4<T> _x, Vec4<T> _y, Vec4<T> _z, Vec4<T> _w)
		: x(_x), y(_y), z(_z), w(_w) {}
};

CORE_NAMESPACE_END

#include <core/math/mat4.inl>

// Export Mat4 out of core namespace
using Mat4f32 = core::Mat4<f32>;
using Mat4f64 = core::Mat4<f64>;
