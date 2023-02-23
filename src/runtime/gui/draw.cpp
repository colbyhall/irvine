// Copyright Colby Hall. All Rights Reserved.

#include <gui/draw.h>

GUI_NAMESPACE_BEGIN

Canvas& Canvas::rect(
	Aabb2f32 bounds, 
	LinearColor color, 
	RectShape shape
) {
	Shape result = {};
	result.bounds = bounds;
	result.color = color;
	result.type = ShapeType::Rect;
	result.rect = shape;

	m_shapes.push(core::move(result));
	return *this;
}

Canvas& Canvas::stroke(
	Aabb2f32 bounds, 
	LinearColor color, 
	StrokeShape shape
) {
	Shape result = {};
	result.bounds = bounds;
	result.color = color;
	result.type = ShapeType::Stroke;
	result.stroke = shape;

	m_shapes.push(core::move(result));
	return *this;
}

Canvas& Canvas::text(
	Aabb2f32 bounds, 
	LinearColor color, 
	TextShape&& shape
) {
	Shape result = {};
	result.bounds = bounds;
	result.color = color;
	result.type = ShapeType::Text;
	result.text = core::forward<TextShape>(shape);

	m_shapes.push(core::move(result));
	return *this;
}

GUI_NAMESPACE_END
