// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.hpp>
#include <core/math/aabb2.hpp>
#include <core/math/color.hpp>
#include <core/math/vec2.hpp>
#include <core/math/vec3.hpp>
#include <core/math/vec4.hpp>

#include <draw/draw.hpp>

DRAW_NAMESPACE_BEGIN

class Shape;

struct Vertex {
    Vec3f32 position;
    LinearColor color;
    Vec4f32 scissor;
    Vec2f32 uv;
    u32 texture;
};

class Canvas {
public:
    static Canvas make() { return Canvas{}; }

    Canvas& add_vertex(const Vec3f32& position, Vec2f32 uv = 0, u32 texture = 0);
    Canvas& paint(const Shape& t);

    FORCE_INLINE Canvas& set_color(const LinearColor& color) { m_color = color; return *this; }
    FORCE_INLINE Canvas& set_scissor(const AABB2f32& scissor) { m_scissor = scissor; return *this; }
    FORCE_INLINE Slice<Vertex const> vertices() const { return m_vertices; }

private:
    explicit Canvas() {}

    LinearColor m_color = LinearColor::white;
    AABB2f32 m_scissor = AABB2f32::from_center(0, { core::F32_MIN, core::F32_MAX });

    Array<Vertex> m_vertices;
};

DRAW_NAMESPACE_END