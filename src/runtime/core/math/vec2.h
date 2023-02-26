// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/option.h>
#include <core/math/math.h>

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

    inline constexpr Vec2() : x(0), y(0) { }
    inline constexpr Vec2(T xy) : x(xy), y(xy) { }
    inline constexpr Vec2(T x, T y) : u(x), v(y) { }

	inline static Vec2<T> from_rad(T theta);

    static const Vec2<T> zero;
    static const Vec2<T> one;
    static const Vec2<T> infinite;

    static const Vec2<T> right;
    static const Vec2<T> up;

    inline T dot(const Vec2<T>& rhs) const { return x * rhs.x + y * rhs.y; }
    inline T cross(const Vec2<T>& rhs) const { return x * rhs.y - y * rhs.x; };
    inline Vec2 perp() const { return { y, -x }; }
    inline Option<Vec2> normalized() const;

    inline T len_sq() const { return dot(*this); }
    inline T len() const { return core::sqrt(len_sq()); }
    inline bool is_zero() const { return *this == zero; }
    inline bool is_near_zero() const { return core::is_near_zero(len_sq()); }
    inline bool has_nan() const { return x == core::nan || y == core::nan; }
    inline bool has_infinite() const { return x == core::infinity || y == core::infinity; }

    template <typename D>
    inline Vec2<D> cast() const { return { (D)x, (D)y }; }

    inline Vec2 operator+(const Vec2& rhs) const { return { x + rhs.x, y + rhs.y }; }
    inline Vec2 operator-(const Vec2& rhs) const { return { x - rhs.x, y - rhs.y }; }
    inline Vec2 operator*(const Vec2& rhs) const { return { x * rhs.x, y * rhs.y }; }
    inline Vec2 operator/(const Vec2& rhs) const { return { x / rhs.x, y / rhs.y }; }
    inline void operator+=(const Vec2& rhs) { *this = *this + rhs; }
    inline void operator-=(const Vec2& rhs) { *this = *this - rhs; }
    inline void operator*=(const Vec2& rhs) { *this = *this * rhs; }
    inline void operator/=(const Vec2& rhs) { *this = *this / rhs; }
    inline Vec2 operator-() const { return { -x, -y }; }
};

CORE_NAMESPACE_END

#include <core/math/vec2.inl>

// Export Vec2 with default types out of core namespace
using Vec2f32 = core::Vec2<f32>;
using Vec2f64 = core::Vec2<f64>;
using Vec2i32 = core::Vec2<i32>;
using Vec2u32 = core::Vec2<u32>;

