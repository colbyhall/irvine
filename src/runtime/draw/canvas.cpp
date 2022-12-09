// Copyright Colby Hall. All Rights Reserved.

#include <draw/canvas.hpp>
#include <draw/shape.hpp>

DRAW_NAMESPACE_BEGIN

Canvas& Canvas::paint(const Shape& t) {
	t.triangulate(*this);
	return *this;
}

Canvas& Canvas::add_vertex(const Vec3f32& position, Vec2f32 uv, u32 texture) {
	Vec4f32 scissor = 0;
	const auto bottom_left = m_scissor.bottom_left();
	scissor.x = bottom_left.x;
	scissor.y = bottom_left.y;

	const auto top_right = m_scissor.top_right();
	scissor.z = top_right.x;
	scissor.w = top_right.y;

	m_vertices.push({
		position,
		m_color,
		scissor,
		uv,
		texture
	});
	return *this;
}


DRAW_NAMESPACE_END
