// Copyright Colby Hall. All Rights Reserved.

#include <draw/canvas.hpp>
#include <draw/font.hpp>
#include <draw/shapes/rect.hpp>
#include <draw/shapes/text.hpp>

DRAW_NAMESPACE_BEGIN

void Text::triangulate(Canvas& canvas) const {
	const auto origin = m_bounds.top_left();
	const f32 size = Font::sdf_size;
	const f32 scale = m_size / size;
	const f32 new_line = (m_font.ascent() - m_font.descent()) + m_font.line_gap();

	auto position = origin;
	position.y -= size * scale;

	for (auto iter = m_text.codepoints(); iter; ++iter) {
		auto c = *iter;

		switch (c) {
			case '\n': {
				position.x = origin.x;
				position.y -= new_line * scale;
			} break;
			case '\r': {
				position.x = origin.x;
			} break;
			case '\t': {
				auto& space_glyph = m_font.glyph(' ');
				position.x += space_glyph.advance * scale;
			} break;
			default:
				auto& glyph = m_font.glyph(c);

				const auto xy = position + glyph.bearing * scale;
				Rect rect = AABB2f32::from_min_max(xy, xy + glyph.size * scale);
				rect.set_texture(m_font.atlas(), glyph.uv0, glyph.uv1);
				canvas.paint(rect);
				position.x += glyph.advance * scale;

				auto peek = iter;
				if (peek && !m_monospace) {
					++peek;

					const auto next = *peek;
					const auto kern = m_font.find_kerning(c, next) * scale;
					if (kern != 0.f) {
						position.x += kern;
					}
				}

				break;
		}
	};
}

DRAW_NAMESPACE_END
