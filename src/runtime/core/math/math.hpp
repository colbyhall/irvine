// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

CORE_NAMESPACE_BEGIN

template <typename T>
constexpr FORCE_INLINE T min(T a, T b) {
    return a < b ? a : b;
}

template <typename T>
constexpr FORCE_INLINE T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
constexpr FORCE_INLINE T clamp(T value, T min, T max) {
    return core::min<T>(core::max<T>(value, min), max);
}

CORE_NAMESPACE_END