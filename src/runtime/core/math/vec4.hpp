// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/vec3.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
struct Vec4 {
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        Vec3<T> xyz;
        T xyzw[4];
    };

    FORCE_INLINE constexpr Vec4() : x(0), y(0), z(0), w(0) { }
    FORCE_INLINE constexpr Vec4(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }
    FORCE_INLINE constexpr Vec4(T x, T y, T z, T w) : r(x), g(y), b(z), a(w) { }
    FORCE_INLINE constexpr Vec4(Vec3<T> xyz, T w) : r(xyz.x), g(xyz.y), b(xyz.z), a(w) { }

    FORCE_INLINE T dot(const Vec4& rhs) const;
    FORCE_INLINE Option<Vec4<T>> normalized() const;

    FORCE_INLINE T len_sq() const { return dot(*this); }
    FORCE_INLINE T len() const { return core::sqrt(len_sq()); }
    FORCE_INLINE bool is_near_zero() const { return core::is_near_zero(len_sq()); }
    FORCE_INLINE bool has_nan() const { return x == core::nan || y == core::nan || z == core::nan || w == core::nan; }
    FORCE_INLINE bool has_infinite() const { return x == core::infinity || y == core::infinity || z == core::infinity || w == core::infinity; }

    template <typename D>
    FORCE_INLINE Vec4<D> cast() const { return { (D)x, (D)y, (D)z, (D)w }; }

    FORCE_INLINE Vec4 operator+(const Vec4& b) const { return { x + b.x, y + b.y, z + b.z, w + b.w }; }
    FORCE_INLINE Vec4 operator-(const Vec4& b) const { return { x - b.x, y - b.y, z - b.z, w - b.w }; }
    FORCE_INLINE Vec4 operator*(const Vec4& b) const { return { x * b.x, y * b.y, z * b.z, w * b.w }; }
    FORCE_INLINE Vec4 operator/(const Vec4& b) const { return { x / b.x, y / b.y, z / b.z, w / b.w }; }
    FORCE_INLINE void operator+=(const Vec4& b) { *this = *this + b; }
    FORCE_INLINE void operator-=(const Vec4& b) { *this = *this - b; }
    FORCE_INLINE void operator*=(const Vec4& b) { *this = *this * b; }
    FORCE_INLINE void operator/=(const Vec4& b) { *this = *this / b; }
};

CORE_NAMESPACE_END

#include <core/math/vec4.inl>

// Export Vec4 out of core namespace
using Vec4f32 = core::Vec4<f32>;
using Vec4f64 = core::Vec4<f64>;
using Vec4i32 = core::Vec4<i32>;
using Vec4u32 = core::Vec4<u32>;