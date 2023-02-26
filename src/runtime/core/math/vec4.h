// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/vec3.h>

CORE_NAMESPACE_BEGIN

template <typename T>
struct Vec4 {
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        Vec3<T> xyz;
        T xyzw[4];
    };

    inline constexpr Vec4() : x(0), y(0), z(0), w(0) { }
    inline constexpr Vec4(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }
    inline constexpr Vec4(T x, T y, T z, T w) : r(x), g(y), b(z), a(w) { }
    inline constexpr Vec4(Vec3<T> xyz, T w) : r(xyz.x), g(xyz.y), b(xyz.z), a(w) { }

    inline T dot(const Vec4& rhs) const;
    inline Option<Vec4<T>> normalized() const;

    inline T len_sq() const { return dot(*this); }
    inline T len() const { return core::sqrt(len_sq()); }
    inline bool is_near_zero() const { return core::is_near_zero(len_sq()); }
    inline bool has_nan() const { return x == core::nan || y == core::nan || z == core::nan || w == core::nan; }
    inline bool has_infinite() const { return x == core::infinity || y == core::infinity || z == core::infinity || w == core::infinity; }

    template <typename D>
    inline Vec4<D> cast() const { return { (D)x, (D)y, (D)z, (D)w }; }

    inline Vec4 operator+(const Vec4& b) const { return { x + b.x, y + b.y, z + b.z, w + b.w }; }
    inline Vec4 operator-(const Vec4& b) const { return { x - b.x, y - b.y, z - b.z, w - b.w }; }
    inline Vec4 operator*(const Vec4& b) const { return { x * b.x, y * b.y, z * b.z, w * b.w }; }
    inline Vec4 operator/(const Vec4& b) const { return { x / b.x, y / b.y, z / b.z, w / b.w }; }
    inline void operator+=(const Vec4& b) { *this = *this + b; }
    inline void operator-=(const Vec4& b) { *this = *this - b; }
    inline void operator*=(const Vec4& b) { *this = *this * b; }
    inline void operator/=(const Vec4& b) { *this = *this / b; }
};

CORE_NAMESPACE_END

#include <core/math/vec4.inl>

// Export Vec4 out of core namespace
using Vec4f32 = core::Vec4<f32>;
using Vec4f64 = core::Vec4<f64>;
using Vec4i32 = core::Vec4<i32>;
using Vec4u32 = core::Vec4<u32>;
