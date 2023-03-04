// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/aabb2.h>
#include <gpu/gpu.h>
#include <draw/shape.h>

GPU_NAMESPACE_BEGIN
class Texture;
GPU_NAMESPACE_END

DRAW_NAMESPACE_BEGIN

struct Round {
	f32 bottom_left = 0.f;
	f32 top_left = 0.f;
	f32 top_right = 0.f;
	f32 bottom_right = 0.f;

	Round() = default;
	Round(f32 all) :
		bottom_left(all),
		top_left(all),
		top_right(all),
		bottom_right(all) {}
	Round(
		f32 in_bottom_left,
		f32 in_top_left,
		f32 in_top_right,
		f32 in_bottom_right
	) :
		bottom_left(in_bottom_left),
		top_left(in_top_left),
		top_right(in_top_right),
		bottom_right(in_bottom_right) {}

	f32 max() const;
	Round adjust(Vec2f32 size) const;
};

class Rect final : public Shape {
public:
	inline Rect(const Aabb2f32& bounds)
		: m_bounds(bounds), m_uv0(0), m_uv1(0), m_texture(0) {}

	Rect& texture(const gpu::Texture& texture, Vec2f32 uv0 = 0.f, Vec2f32 uv1 = 1.f);
	Rect& stroke(f32 size);
	Rect& round(Round round);

	// Shape
	void triangulate(Canvas& canvas) const override;
	// ~Shape

private:
	Aabb2f32 m_bounds;

	Option<f32> m_stroke;
	Round m_round;

	Vec2f32 m_uv0, m_uv1;
	u32 m_texture;
};

DRAW_NAMESPACE_END
