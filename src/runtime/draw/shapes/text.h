// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string_view.h>
#include <core/math/aabb2.h>

#include <draw/shape.h>

DRAW_NAMESPACE_BEGIN

class Font;

class Text final : public Shape {
public:
    FORCE_INLINE Text(StringView text, const Aabb2f32& bounds, const Font& font, f32 size)
    : m_bounds(bounds), m_text(text), m_font(font), m_size(size) {}

    FORCE_INLINE Text& monospace() { m_monospace = true; }

    // Shape Interface
    void triangulate(Canvas& canvas) const override;
    // ~Shape Interface

private:
    Aabb2f32 m_bounds;
    StringView m_text;
    const Font& m_font;
    f32 m_size;

    bool m_monospace = false;
};

DRAW_NAMESPACE_END
