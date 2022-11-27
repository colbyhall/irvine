// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/vec2.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Vec3 {
    union {
        struct { T x, y, z; };
        struct { T width, height, depth; };
        struct { T u, v, w; };
        Vec2<T> xy;
        T xyz[3];
    };

    FORCE_INLINE constexpr Vec3() : x(0), y(0), z(0) { }
    FORCE_INLINE constexpr Vec3(T xyz) : x(xyz), y(xyz), z(xyz) { }
    FORCE_INLINE constexpr Vec3(T x, T y, T z) : u(x), v(y), w(z) { }
    FORCE_INLINE constexpr Vec3(Vec2<T> xy, T z = (T)0) : u(xy.x), v(xy.y), w(z) { }

    static const Vec3<T> zero;
    static const Vec3<T> one;
    static const Vec3<T> infinite;

    static const Vec3<T> forward;
    static const Vec3<T> right;
    static const Vec3<T> up;

    // Orthonormal basis is based on left handed coordinate system with x - forward, y - right, z - up
    static void orthonormal_basis(Vec3<T>& forward, Vec3<T>& right, Vec3<T>& up);

    FORCE_INLINE T dot(const Vec3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; };
    FORCE_INLINE Vec3<T> cross(const Vec3& rhs) const;
    FORCE_INLINE Option<Vec3<T>> normalized() const;

    FORCE_INLINE T len_sq() const { return dot(*this); }
    FORCE_INLINE T len() const { return core::sqrt(len_sq()); }
    FORCE_INLINE bool is_near_zero() const { return core::is_near_zero(len_sq()); }
    FORCE_INLINE bool has_nan() const { return x == core::nan || y == core::nan || z == core::nan; }
    FORCE_INLINE bool has_infinite() const { return x == core::infinity || y == core::infinity || z == core::infinity; }

    template <typename D>
    FORCE_INLINE Vec3<D> cast() const { return { (D)x, (D)y, (D)z }; }

    FORCE_INLINE Vec3 operator+(const Vec3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    FORCE_INLINE Vec3 operator-(const Vec3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    FORCE_INLINE Vec3 operator*(const Vec3& rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
    FORCE_INLINE Vec3 operator/(const Vec3& rhs) const { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
    FORCE_INLINE void operator+=(const Vec3& rhs) { *this = *this + rhs; }
    FORCE_INLINE void operator-=(const Vec3& rhs) { *this = *this - rhs; }
    FORCE_INLINE void operator*=(const Vec3& rhs) { *this = *this * rhs; }
    FORCE_INLINE void operator/=(const Vec3& rhs) { *this = *this / rhs; }
    FORCE_INLINE Vec3 operator-() const { return { -x, -y, -z }; }
};

CORE_NAMESPACE_END

#include <core/math/vec3.inl>

// Export Vec3 with default types out of core namespace
using Vec3f32 = core::Vec3<f32>;
using Vec3f64 = core::Vec3<f64>;
using Vec3i32 = core::Vec3<i32>;
using Vec3fu32 = core::Vec3<u32>;