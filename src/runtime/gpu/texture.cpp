// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.h>
#include <gpu/d3d12/d3d12_texture.h>

GPU_NAMESPACE_BEGIN

usize format_size_in_bytes(Format format) {
	SUPPRESS_WARNING_PUSH
	MSVC_SUPPRESS_WARNING(4061) // Not all switch cases are explicitly handled
	switch (format) {
	case Format::R_U8:
		return 1;
		break;
		// RGB_U8,
		// RGB_U8_SRGB,
	case Format::RGBA_U8:
	case Format::RGBA_U8_SRGB:
		return 4;
		break;
	case Format::RGBA_F16:
		return 4 * 2;
		break;
	case Format::RGBA_F32:
		return 4 * 4;
		break;
		// BGR_U8_SRGB,
		// Depth16,
	case Format::Depth24_Stencil8:
		return 4;
		break;
	default:
		ASSERT(false, "Unsupported format type");
		break;
	}
	SUPPRESS_WARNING_POP

	return 0;
}

Texture Texture::make(
	TextureUsage usage,
	Format format,
	const Vec3u32& size
) {
	auto& context = Context::the();

	Option<Shared<TextureInterface, SMode::Atomic>> interface;
	switch (context.backend()) {
		case Backend::D3D12:
			interface = make_shared<D3D12Texture, SMode::Atomic>(
				usage,
				format,
				size
			);
			break;
	}

	return Texture { interface.unwrap() };
}

GPU_NAMESPACE_END
