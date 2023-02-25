// Copyright Colby Hall. All Rights Reserved.

#include <gpu/texture.h>

#include <draw/canvas.h>
#include <draw/shapes/rect.h>

DRAW_NAMESPACE_BEGIN

f32 Round::max() const {
	auto result = bottom_left;
	if (result < bottom_right) {
		result = bottom_right;
	}
	if (result < top_left) {
		result = top_left;
	}
	if (result < top_right) {
		result = top_right;
	}
	return result;
}

Round Round::adjust(Vec2f32 size) const {
	auto bl = bottom_left;
	if (bl + top_left > size.y) {
		const auto scale = size.y / (bl + top_left);
		bl *= scale;
	}
	if (bl + bottom_right > size.x) {
		const auto scale = size.x / (bl + bottom_right);
		bl *= scale;
	}

	auto tl = top_left;
	if (tl + bottom_left > size.y) {
		const auto scale = size.y / (tl + bottom_left);
		tl *= scale;
	}
	if (tl + top_right > size.x) {
		const auto scale = size.x / (tl + top_right);
		tl *= scale;
	}

	auto tr = top_right;
	if (tr + bottom_right > size.y) {
		const auto scale = size.y / (tr + bottom_right);
		tr *= scale;
	}
	if (tr + top_left > size.x) {
		const auto scale = size.x / (tr + top_left);
		tr *= scale;
	}

	auto br = bottom_right;
	if (br + top_right > size.y) {
		const auto scale = size.y / (br + top_right);
		br *= scale;
	}
	if (br + bottom_left > size.x) {
		const auto scale = size.x / (br + bottom_left);
		br *= scale;
	}

	return Round {
		bl,
		tl,
		tr,
		br
	};
}

Rect& Rect::texture(const gpu::Texture& texture, Vec2f32 uv0, Vec2f32 uv1) {
	m_texture = texture.bindless();
	m_uv0 = uv0;
	m_uv1 = uv1;
	return *this;
}

Rect& Rect::stroke(f32 size) {
	m_stroke = size;
	return *this;
}

Rect& Rect::round(Round round) {
	m_round = round;
	return *this;
}

void Rect::triangulate(Canvas& canvas) const {
	if (m_stroke) {
		ASSERT(false, "TODO");
	}
	else {
		// If we have any round corners draw a rectangle similar to a circle
		if (m_round.max() > 0.f) {
			auto find_uv = [this](Vec2f32 position) {
				return (position - m_bounds.min()) / m_bounds.size();
			};
			const auto center_index = canvas.add_vertex(
				m_bounds.position(),
				find_uv(m_bounds.position()),
				m_texture
			);

			const auto frac_pi_2 = core::pi<f32> / 2.f;

			auto draw_corner = [&](f32 size, Vec2f32 origin, f32 theta) {
				const auto sides = 6;
				const auto increment = frac_pi_2 / (f32)sides;
				for (i32 i = 0; i < sides; ++i) {
					const auto a = Vec2f32::from_rad(
						(f32)i * increment + theta
					) * size + origin;
					const auto b = Vec2f32::from_rad(
						(f32)(i + 1) * increment + theta
					) * size + origin;

					const auto a_index = canvas.add_vertex(
						a,
						find_uv(a),
						m_texture
					);

					const auto b_index = canvas.add_vertex(
						b,
						find_uv(b),
						m_texture
					);

					canvas.add_index(a_index);
					canvas.add_index(b_index);
					canvas.add_index(center_index);
				}
			};

			const auto adjusted = m_round.adjust(m_bounds.size());
			draw_corner(
				adjusted.bottom_left,
				m_bounds.bottom_left() + adjusted.bottom_left,
				frac_pi_2 * 2.f
			);
			draw_corner(
				adjusted.top_left,
				m_bounds.top_left() + Vec2f32 {
					adjusted.top_left,
					-adjusted.top_left
				},
				frac_pi_2 * 3.f
			);
			draw_corner(
				adjusted.top_right,
				m_bounds.top_right() - adjusted.top_right,
				0.f
			);
			draw_corner(
				adjusted.bottom_right,
				m_bounds.bottom_right() + Vec2f32{
					-adjusted.bottom_right,
					adjusted.bottom_right
				},
				frac_pi_2
			);

			auto draw_sides = [&](Vec2f32 a, Vec2f32 b) {
				const auto a_index = canvas.add_vertex(
					a,
					find_uv(a),
					m_texture
				);

				const auto b_index = canvas.add_vertex(
					b,
					find_uv(b),
					m_texture
				);

				canvas.add_index(a_index);
				canvas.add_index(b_index);
				canvas.add_index(center_index);
			};

			// Left side
			draw_sides(
				m_bounds.bottom_left() + Vec2f32 { 0.f, adjusted.bottom_left },
				m_bounds.top_left() + Vec2f32 { 0.f, -adjusted.top_left }
			);

			// Top side
			draw_sides(
				m_bounds.top_left() + Vec2f32 { adjusted.top_left, 0.f },
				m_bounds.top_right() + Vec2f32 { -adjusted.top_right, 0.f }
			);

			// Right side
			draw_sides(
				m_bounds.top_right() + Vec2f32 { 0.f, -adjusted.top_right },
				m_bounds.bottom_right() + Vec2f32 { 0.f, adjusted.bottom_right }
			);

			// Bottom side
			draw_sides(
				m_bounds.bottom_right() + Vec2f32 {
					-adjusted.bottom_right,
					0.f
				},
				m_bounds.bottom_left() + Vec2f32 { adjusted.bottom_left, 0.f }
			);
		}
		// Otherwise draw the simplest quad we can
		else {
			const auto bottom_left = canvas.add_vertex(
				m_bounds.bottom_left(),
				m_uv0,
				m_texture
			);

			const auto top_left = canvas.add_vertex(
				m_bounds.top_left(),
				{ m_uv0.x, m_uv1.y },
				m_texture
			);

			const auto top_right = canvas.add_vertex(
				m_bounds.top_right(),
				m_uv1,
				m_texture
			);

			const auto bottom_right = canvas.add_vertex(
				m_bounds.bottom_right(),
				{ m_uv1.x, m_uv0.y },
				m_texture
			);

			canvas.add_index(bottom_left);
			canvas.add_index(top_left);
			canvas.add_index(top_right);

			canvas.add_index(bottom_left);
			canvas.add_index(top_right);
			canvas.add_index(bottom_right);
		}
	}
}

DRAW_NAMESPACE_END
