// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/aabb2.h>
#include <gpu/gpu.h>
#include <draw/shape.h>

GPU_NAMESPACE_BEGIN
class Texture;
GPU_NAMESPACE_END

DRAW_NAMESPACE_BEGIN

class Rect final : public Shape {
public:
    FORCE_INLINE Rect(const AABB2f32& bounds)
        : m_bounds(bounds), m_uv0(0), m_uv1(0), m_texture(0) {}

    Rect& set_texture(const gpu::Texture& texture, Vec2f32 uv0 = 0.f, Vec2f32 uv1 = 1.f);

    // Shape
    void triangulate(Canvas& canvas) const override;
    // ~Shape

private:
    AABB2f32 m_bounds;

    Vec2f32 m_uv0, m_uv1;
    u32 m_texture;
};

DRAW_NAMESPACE_END
