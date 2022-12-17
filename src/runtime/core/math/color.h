// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

CORE_NAMESPACE_BEGIN

struct LinearColor {
    f32 r, g, b, a;

    FORCE_INLINE constexpr LinearColor()
        : r(1), g(1), b(1), a(1) {}
    FORCE_INLINE constexpr LinearColor(f32 in_r, f32 in_g, f32 in_b, f32 in_a)
        : r(in_r), g(in_g), b(in_b), a(in_a) {}

    static const LinearColor red;
    static const LinearColor green;
    static const LinearColor blue;
    static const LinearColor white;
    static const LinearColor black;
};

struct sRGBColor {
    union {
        struct { u8 r, g, b, a; };
        u32 rgba;
    };
};

CORE_NAMESPACE_END

// Export LinearColor and sRGBColor out of core namespace
using core::LinearColor;
using core::sRGBColor;
