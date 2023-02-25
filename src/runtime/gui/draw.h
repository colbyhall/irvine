// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gui/gui.h>
#include <core/math/aabb2.h>
#include <core/math/color.h>
#include <core/containers/string.h>
#include <draw/shapes/rect.h>

GUI_NAMESPACE_BEGIN

using draw::Round;

enum class ShapeType : u8 {
	Rect,
	Text,
};

struct RectShape {
	Round round;
	Option<f32> stroke;
};

struct TextShape {
	String text;
	f32 size;
};

struct Shape {
	Aabb2f32 bounds;
	LinearColor color;
	ShapeType type;

	RectShape rect;
	TextShape text;
};

class Canvas {
public:
	FORCE_INLINE static Canvas make() { return Canvas(); }

	Canvas& rect(Aabb2f32 bounds, LinearColor color, RectShape shape = {});
	Canvas& text(Aabb2f32 bounds, LinearColor color, TextShape&& shape);

	FORCE_INLINE Slice<Shape const> shapes() const { return m_shapes; }

private:
	Canvas() = default;

	Array<Shape> m_shapes;
};

GUI_NAMESPACE_END
