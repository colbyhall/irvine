// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/shared.hpp>
#include <core/math/vec3.hpp>

#include <gpu/gpu.hpp>

GPU_NAMESPACE_BEGIN

enum class Filter : u8 {
    Nearest,
    Linear
};

enum class Wrap : u8 {
    Clamp,
    Repeat,
};

struct Sampler {
    Filter min_filter = Filter::Linear;
    Filter mag_filter = Filter::Linear;

    Wrap address_u = Wrap::Clamp;
    Wrap address_v = Wrap::Clamp;
    Wrap address_w = Wrap::Clamp;
};

BITFLAG(TextureUsage) {
    TU_TransferSrc = (1 << 0),
    TU_TransferDst = (1 << 1),
    TU_Sampled = (1 << 2),
    TU_ColorAttachment = (1 << 3),
    TU_DepthAttachment = (1 << 4),
    TU_Swapchain = (1 << 5),
};

enum class Format : u16{
    Undefined,
    
    R_U8,

    RGB_U8,
    RGB_U8_SRGB,
    RGBA_U8,
    RGBA_U8_SRGB,

    RGBA_F16,

    RGBA_F32,

    BGR_U8_SRGB,

    Depth16,
    Depth24_Stencil8,
};

usize format_size_in_bytes(Format format);

class TextureInterface : private NonCopyable {
public:
    virtual TextureUsage usage() const = 0;
    virtual Format format() const = 0;
    virtual Vec3u32 size() const = 0;
    virtual u32 bindless() const = 0;
    virtual ~TextureInterface() {}
};

class Texture {
public:
    static Texture make(
        TextureUsage usage,
        Format format,
        const Vec3u32& size
    );

    FORCE_INLINE TextureUsage usage() const { return m_interface->usage(); }
    FORCE_INLINE Format format() const { return m_interface->format(); }
    FORCE_INLINE Vec3u32 size() const { return m_interface->size(); }
    FORCE_INLINE u32 bindless() const { return m_interface->bindless(); }

    template <typename T = TextureInterface>
    FORCE_INLINE T const& interface() const {
        static_assert(core::is_base_of<TextureInterface, T>, "T is not derived of TextureInterface");
        return static_cast<const T&>(*m_interface);
    }

    FORCE_INLINE Texture clone() const { return m_interface.clone(); }

private:
    Texture(Shared<TextureInterface, SMode::Atomic>&& interface) : m_interface(core::move(interface)) { }

    friend class D3D12Swapchain;

    Shared<TextureInterface, SMode::Atomic> m_interface;
};

GPU_NAMESPACE_END