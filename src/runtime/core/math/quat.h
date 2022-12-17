// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/option.h>
#include <core/math/vec3.h>

CORE_NAMESPACE_BEGIN

template <typename T>
struct Quat {
    T i, j, k, w;

    FORCE_INLINE constexpr Quat() : i(0), j(0), k(0), w(1) {}
    FORCE_INLINE explicit Quat(T _i, T _j, T _k, T _w) : i(_i), j(_j), k(_k), w(_w) {}
    static Quat from_axis_angle(Vec3<T> axis, T theta);
    static Quat from_euler(T pitch, T yaw, T roll);

    static const Quat identity;

    Option<Quat> normalized() const;
    FORCE_INLINE Quat inverse() const { return Quat{ -i, -j, -k, w }; }
    Vec3<T> rotate_vector(const Vec3<T>& xyz) const;

    FORCE_INLINE T len_sq() const { return i * i + j * j + k * k + w * w; }
    FORCE_INLINE T len() const { return core::sqrt(len_sq()); }
    FORCE_INLINE bool is_near_zero() const { return core::is_near_zero(len_sq()); }

    Quat operator*(const Quat& rhs) const;
};

CORE_NAMESPACE_END

#include <core/math/quat.inl>

// Export Quat out of core namespace
using Quatf32 = core::Quat<f32>;
using Quatf64 = core::Quat<f64>;
