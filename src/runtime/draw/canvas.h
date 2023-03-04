// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.h>
#include <core/math/aabb2.h>
#include <core/math/color.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>

#include <draw/draw.h>

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

	u32 add_vertex(const Vec3f32& position, Vec2f32 uv = 0, u32 texture = 0);
	void add_index(u32 index);

	Canvas& paint(const Shape& t);

	inline Canvas& set_color(const LinearColor& color) { m_color = color; return *this; }
	inline Canvas& set_scissor(const Aabb2f32& scissor) { m_scissor = scissor; return *this; }

	inline Slice<Vertex const> vertices() const { return m_vertices; }
	inline Slice<u32 const> indices() const { return m_indices; }

private:
	explicit Canvas() {}

	LinearColor m_color = LinearColor::white;
	Aabb2f32 m_scissor = Aabb2f32::from_center(0, { core::F32_MIN, core::F32_MAX });

	Array<Vertex> m_vertices;
	Array<u32> m_indices;
};

DRAW_NAMESPACE_END
