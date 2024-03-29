// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.h>
#include <core/containers/option.h>
#include <core/math/vec2.h>
#include <core/fs/path_view.h>

#include <gpu/texture.h>

#include <draw/draw.h>

struct stbtt_fontinfo;

DRAW_NAMESPACE_BEGIN

struct Glyph {
	Vec2f32 size;
	Vec2f32 bearing;
	Vec2f32 uv0, uv1;
	f32 advance;
};

// TODO: Store own kerning table
class Font {
public:
	static constexpr inline f32 sdf_size = 32.f;
	static Option<Font> import(PathView path);

	inline Glyph const& glyph(Char c) const {
		const auto index = m_codepoints_to_glyphs[(usize)c];
		if (!m_glphys.is_valid_index(index)) return m_glphys[0];
		return m_glphys[(usize)index];
	}
	inline gpu::Texture const& atlas() const { return m_atlas; }

	inline f32 ascent() const { return m_ascent; }
	inline f32 descent() const { return m_descent; }
	inline f32 line_gap() const { return m_line_gap; }
	inline f32 new_line() const { return (ascent() - descent()) + line_gap(); };
	inline f32 scale(f32 size) { return size / sdf_size; }

	f32 find_kerning(Char a, Char b) const;

	Vec2f32 text_size(StringView text, f32 size, bool monospace = false) const;

private:
	Font(
		Array<u8>&& bytes,
		const stbtt_fontinfo* info,
		Array<Glyph>&& glyphs,
		Array<u32>&& codepoints_to_glyphs,
		gpu::Texture&& atlas,
		f32 ascent,
		f32 descent,
		f32 line_gap,
		f32 scale
	);

	Array<u8> m_bytes;
	stbtt_fontinfo* m_info; // TODO: free memory on destruction

	Array<Glyph> m_glphys;
	Array<u32> m_codepoints_to_glyphs; // Codepoint to glyph map
	gpu::Texture m_atlas;

	f32 m_ascent;
	f32 m_descent;
	f32 m_line_gap;

	f32 m_scale; // stb scale used for kerning
};

DRAW_NAMESPACE_END
