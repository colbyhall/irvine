// Copyright Colby Hall. All Rights Reserved.

#include <core/memory.hpp>

#include <gpu/buffer.hpp>
#include <gpu/graphics_command_list.hpp>

using namespace gpu;

#include <draw/font.hpp>

SUPPRESS_WARNINGS_STD_BEGIN
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
SUPPRESS_WARNINGS_STD_END

DRAW_NAMESPACE_BEGIN

Option<Font> Font::import(PathView path) {
	auto result = File::open(path, FF_Read);
	if (!result.is_ok()) return nullptr;
	File file = result.unwrap();
	const auto file_size = file.size();

	Array<u8> bytes;
	bytes.reserve(file_size + 1);
	bytes.set_len(file_size);
	file.read({ &bytes[0], bytes.len() });
	bytes.push(0);

	stbtt_fontinfo info = {};
	stbtt_InitFont(&info, &bytes[0], stbtt_GetFontOffsetForIndex(&bytes[0], 0));

	Array<Glyph> glyphs;
	glyphs.reserve(info.numGlyphs);

	const auto scale = stbtt_ScaleForPixelHeight(&info, Font::sdf_size);
	const auto padding = 5;
	const auto onedge_value = 180;
	const auto pixel_dist_scale = (f32)onedge_value / (f32)padding;

	int ascent, descent, line_gap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);

	const f32 scaled_ascent = (f32)ascent * scale;
	const f32 scaled_descent = (f32)descent * scale;
	const f32 scaled_line_gap = (f32)line_gap * scale;

	const auto atlas_size = Vec2u32{ 512, 512 };

	Array<stbrp_node> nodes;
	nodes.reserve(atlas_size.width * atlas_size.height);

	stbrp_context packer = {};
	stbrp_init_target(&packer, atlas_size.width, atlas_size.height, nodes.begin(), (int)nodes.cap());

	Array<u8> bitmap;
	bitmap.reserve(atlas_size.width * atlas_size.height);
	bitmap.set_len(atlas_size.width * atlas_size.height);
	core::mem::set(&bitmap[0], 0, bitmap.len());

	for (int glyph = 0; glyph < 256; ++glyph) {
		int width, height, xoff, yoff;
		auto* sdf_bitmap = stbtt_GetGlyphSDF(
			&info,
			scale,
			glyph,
			padding,
			onedge_value,
			pixel_dist_scale,
			&width,
			&height,
			&xoff,
			&yoff
		);

		if (!sdf_bitmap) {
			glyphs.push(Glyph {});
			continue;
		}

		stbrp_rect rect = {};
		rect.w = width;
		rect.h = height;

		if (stbrp_pack_rects(&packer, &rect, 1)) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					const auto z = height - y;
					bitmap[(x + rect.x) + (z + rect.y) * (int)atlas_size.width] = sdf_bitmap[x + y * width];
				}
			}
		}

		stbtt_FreeSDF(sdf_bitmap, nullptr);

		const auto size = Vec2f32 { (f32) width, (f32)height };
		const auto bearing = Vec2f32 { (f32)(xoff), -(f32)((height + yoff)) };
		const auto uv0 = Vec2f32 {
			(f32)rect.x / (f32)atlas_size.width,
			(f32)rect.y / (f32)atlas_size.height
		};
		const auto uv1 = Vec2f32{
			(f32)(rect.x + rect.w) / (f32)atlas_size.width,
			(f32)(rect.y + rect.h) / (f32)atlas_size.height
		};
		glyphs.push(Glyph {
			size,
			bearing,
			uv0, uv1,
			0.f
		});
	}

	auto pixels_buffer = Buffer::make(
		BU_TransferSrc,
		BufferKind::Upload,
		bitmap.len(),
		sizeof(u8)
	);
	pixels_buffer.map([&bitmap](Slice<u8> slice) {
		core::mem::copy(&slice[0], &bitmap[0], slice.len());
	});

	auto pixels_texture = Texture::make(
		TU_TransferDst | TU_Sampled,
		Format::R_U8,
		{ atlas_size.width, atlas_size.height, 1 }
	);

	auto command_list = GraphicsCommandList::make();
	command_list.record([&](GraphicsCommandRecorder& gcr) {
		gcr.texture_barrier(pixels_texture, Layout::General, Layout::TransferDst);
		gcr.copy_buffer_to_texture(pixels_texture, pixels_buffer);
		gcr.texture_barrier(pixels_texture, Layout::TransferDst, Layout::General);
	});
	command_list.submit();

	Array<Codepoint> codepoints_to_glyphs;
	codepoints_to_glyphs.reserve(0x110000);

	for (Codepoint c = 0; c < 0x110000; ++c) {
		const auto index = stbtt_FindGlyphIndex(&info, (int)c);
		codepoints_to_glyphs.push(index == -1 ? 0 : (u32)index);

		int advance, left_bearing;
		stbtt_GetCodepointHMetrics(&info, c, &advance, &left_bearing);

		if (glyphs.is_valid_index((usize)index)) {
			glyphs[(usize)index].advance = (f32)advance * scale;
		}
	}

	return Font {
		core::move(bytes),
		&info,
		core::move(glyphs),
		core::move(codepoints_to_glyphs),
		core::move(pixels_texture),
		scaled_ascent,
		scaled_descent,
		scaled_line_gap,
		scale
	};
}

Font::Font(
	Array<u8>&& bytes,
	const stbtt_fontinfo* info,
	Array<Glyph>&& glyphs,
	Array<u32>&& codepoints_to_glyphs,
	Texture&& atlas,
	f32 ascent,
	f32 descent,
	f32 line_gap,
	f32 scale
) :
	m_bytes(core::move(bytes)),
	m_glphys(core::move(glyphs)),
	m_codepoints_to_glyphs(core::move(codepoints_to_glyphs)),
	m_atlas(core::move(atlas)),
	m_ascent(ascent),
	m_descent(descent),
	m_line_gap(line_gap),
	m_scale(scale) {

	m_info = new stbtt_fontinfo;
	*m_info = *info;
}

f32 Font::find_kerning(Codepoint a, Codepoint b) const {
	return (f32)stbtt_GetCodepointKernAdvance(m_info, (int)a, (int)b) * m_scale;
}

DRAW_NAMESPACE_END
