// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gui/gui.h>
#include <core/math/aabb2.h>
#include <core/math/color.h>
#include <core/containers/string.h>

GUI_NAMESPACE_BEGIN

struct Round {
	f32 bottom_left = 0.f;
	f32 top_left = 0.f;
	f32 top_right = 0.f;
	f32 bottom_right = 0.f;
};

enum class ShapeType : u8 {
	Rect,
	Stroke,
	Text,
};

struct RectShape {
	Round round = {};
};

struct StrokeShape {
	Round round = {};
	f32 size;
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
	StrokeShape stroke;
	TextShape text;
};

class Canvas {
public:
	FORCE_INLINE static Canvas make() { return Canvas(); }

	Canvas& rect(Aabb2f32 bounds, LinearColor color, RectShape shape = {});
	Canvas& stroke(Aabb2f32 bounds, LinearColor color, StrokeShape shape);
	Canvas& text(Aabb2f32 bounds, LinearColor color, TextShape&& shape);

	FORCE_INLINE Slice<Shape const> shapes() const { return m_shapes; }

private:
	Canvas() = default;

	Array<Shape> m_shapes;
};

GUI_NAMESPACE_END
