// Copyright Colby Hall. All Rights Reserved.

CORE_NAMESPACE_BEGIN

template <typename T>
const Mat3<T> Mat3<T>::identity = Mat3<T>::from_columns(
	{ 1, 0, 0 },
	{ 0, 1, 0 },
	{ 0, 0, 1 }
);

template <typename T>
constexpr Mat3<T> Mat3<T>::from_columns(Vec3<T> x, Vec3<T> y, Vec3<T> z) {
	return { x, y, z };
}

template <typename T>
constexpr Mat3<T> Mat3<T>::from_rows(Vec3<T> x, Vec3<T> y, Vec3<T> z) {
	const Vec3<T> x_col = { x.x, y.x, z.x };
	const Vec3<T> y_col = { x.y, y.y, z.y };
	const Vec3<T> z_col = { x.z, y.z, z.z };
	return { x_col, y_col, z_col};
}

template <typename T>
Vec3<T> Mat3<T>::row(usize index) const {
	ASSERT(index < 3);
	switch (index) {
	case 0:
		return { x.x, y.x, z.x };
	case 1:
		return { x.y, y.y, z.y };
	case 2:
		return { x.z, y.z, z.z };
	}
	return {};
}

template <typename T>
Mat3<T> Mat3<T>::operator*(const Mat3<T>& rhs) const {
	Vec3<T> row_x = {};
	const auto row0 = row(0);
	row_x.x = row0.dot(rhs.x);
	row_x.y = row0.dot(rhs.y);
	row_x.z = row0.dot(rhs.z);

	Vec3<T> row_y = {};
	const auto row1 = row(1);
	row_y.x = row1.dot(rhs.x);
	row_y.y = row1.dot(rhs.y);
	row_y.z = row1.dot(rhs.z);

	Vec3<T> row_z = {};
	const auto row2 = row(2);
	row_z.x = row2.dot(rhs.x);
	row_z.y = row2.dot(rhs.y);
	row_z.z = row2.dot(rhs.z);

	return Mat3::from_rows(row_x, row_y, row_z);
}

template <typename T>
Vec3<T> Mat3<T>::operator*(const Vec3<T>& rhs) const {
	const auto x = row(0).dot(rhs);
	const auto y = row(1).dot(rhs);
	const auto z = row(2).dot(rhs);

	return { x, y, z };
}

CORE_NAMESPACE_END
