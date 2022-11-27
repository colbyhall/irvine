// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/option.hpp>
#include <core/math/math.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
struct Vec2 {
    // This probably has issues with undefined behavior but it works
    union {
        struct { T x, y; };
        struct { T width, height; };
        struct { T u, v; };
        T xy[2];
    };

    FORCE_INLINE constexpr Vec2() : x(0), y(0) { }
    FORCE_INLINE constexpr Vec2(T xy) : x(xy), y(xy) { }
    FORCE_INLINE constexpr Vec2(T x, T y) : u(x), v(y) { }

    static const Vec2<T> zero;
    static const Vec2<T> one;
    static const Vec2<T> infinite;

    static const Vec2<T> right;
    static const Vec2<T> up;

    FORCE_INLINE T dot(const Vec2<T>& rhs) const { return x * rhs.x + y * rhs.y; }
    FORCE_INLINE T cross(const Vec2<T>& rhs) const { return x * rhs.y - y * rhs.x; };
    FORCE_INLINE Vec2 perp() const { return { y, -x }; }
    FORCE_INLINE Option<Vec2> normalized() const;

    FORCE_INLINE T len_sq() const { return dot(*this); }
    FORCE_INLINE T len() const { return core::sqrt(len_sq()); }
    FORCE_INLINE bool is_zero() const { return *this == zero; }
    FORCE_INLINE bool is_near_zero() const { return core::is_near_zero(len_sq()); }
    FORCE_INLINE bool has_nan() const { return x == core::nan || y == core::nan; }
    FORCE_INLINE bool has_infinite() const { return x == core::infinity || y == core::infinity; }

    template <typename D>
    FORCE_INLINE Vec2<D> cast() const { return { (D)x, (D)y }; }

    FORCE_INLINE Vec2 operator+(const Vec2& rhs) const { return { x + rhs.x, y + rhs.y }; }
    FORCE_INLINE Vec2 operator-(const Vec2& rhs) const { return { x - rhs.x, y - rhs.y }; }
    FORCE_INLINE Vec2 operator*(const Vec2& rhs) const { return { x * rhs.x, y * rhs.y }; }
    FORCE_INLINE Vec2 operator/(const Vec2& rhs) const { return { x / rhs.x, y / rhs.y }; }
    FORCE_INLINE void operator+=(const Vec2& rhs) { *this = *this + rhs; }
    FORCE_INLINE void operator-=(const Vec2& rhs) { *this = *this - rhs; }
    FORCE_INLINE void operator*=(const Vec2& rhs) { *this = *this * rhs; }
    FORCE_INLINE void operator/=(const Vec2& rhs) { *this = *this / rhs; }
    FORCE_INLINE Vec2 operator-() const { return { -x, -y }; }
};

CORE_NAMESPACE_END

#include <core/math/vec2.inl>

// Export Vec2 with default types out of core namespace
using Vec2f32 = core::Vec2<f32>;
using Vec2f64 = core::Vec2<f64>;
using Vec2i32 = core::Vec2<i32>;
using Vec2fu32 = core::Vec2<u32>;

