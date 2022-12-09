// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string_view.hpp>
#include <core/math/aabb2.hpp>

#include <draw/shape.hpp>

DRAW_NAMESPACE_BEGIN

class Font;

class Text final : public Shape {
public:
    FORCE_INLINE Text(StringView text, const AABB2f32& bounds, const Font& font, f32 size)
    : m_bounds(bounds), m_text(text), m_font(font), m_size(size) {}

    FORCE_INLINE Text& monospace() { m_monospace = true; }

    // Shape Interface
    void triangulate(Canvas& canvas) const override;
    // ~Shape Interface

private:
    AABB2f32 m_bounds;
    StringView m_text;
    const Font& m_font;
    f32 m_size;

    bool m_monospace = false;
};

DRAW_NAMESPACE_END