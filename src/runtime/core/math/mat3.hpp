// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/vec3.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
struct Mat3 {
    union {
        struct { Vec3<T> x, y, z; };
        Vec3<T> columns[3];
        T elements[3 * 3];
    };

    FORCE_INLINE constexpr Mat3() : x(0), y(0), z(0) {}
    static constexpr Mat3 from_columns(Vec3<T> x, Vec3<T> y, Vec3<T> z);
    static constexpr Mat3 from_rows(Vec3<T> x, Vec3<T> y, Vec3<T> z);
    
    static const Mat3 identity;

    // TODO: Option<Mat3> inverse() const;

    Vec3<T> row(usize index) const;

    Mat3 operator*(const Mat3& rhs) const;
    Vec3<T> operator*(const Vec3<T>& rhs) const;

private:
    FORCE_INLINE constexpr Mat3(Vec3<T> _x, Vec3<T> _y, Vec3<T> _z)
        : x(_x), y(_y), z(_z) {}
};

CORE_NAMESPACE_END

#include <core/math/mat3.inl>

// Export Mat3 out of core namespace
using Mat3f32 = core::Mat3<f32>;
using Mat3f64 = core::Mat3<f64>;