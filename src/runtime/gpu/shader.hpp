// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/slice.hpp>
#include <core/containers/string.hpp>
#include <core/containers/shared.hpp>

#include <gpu/gpu.hpp>

GPU_NAMESPACE_BEGIN

enum class ShaderType : u8 {
    Vertex,
    Pixel,
};

class ShaderInterface : private NonCopyable {
public:
    virtual Slice<u8 const> binary() const = 0;
    virtual ShaderType type() const = 0;
    virtual ~ShaderInterface() {}
};

enum class Primitive : u8 {
    Uint32,
    Int32,
    Float32,
    Vec2f32,
    Vec3f32,
    Vec4f32,
    Mat4f32,
};

struct InputParameter {
    u32 semantic_index;
    String semantic_name;
    Primitive primitive;
};

class VertexShaderInterface : public ShaderInterface {
public:
    ShaderType type() const override { return ShaderType::Vertex; }
    virtual Slice<InputParameter const> input_parameters() const = 0;
};

class VertexShader {
public:
    static VertexShader make(Array<u8>&& binary, Array<InputParameter>&& input_parameters);

    inline Slice<const u8> binary() const { return m_interface->binary(); }
    inline ShaderType type() const { return m_interface->type(); }
    inline Slice<InputParameter const> input_parameters() const { return m_interface->input_parameters(); }

private:
    VertexShader(Shared<VertexShaderInterface, SMode::Atomic>&& interface) : m_interface(core::move(interface)) { }

    Shared<VertexShaderInterface, SMode::Atomic> m_interface;
};

class PixelShaderInterface : public ShaderInterface {
public:
    ShaderType type() const override { return ShaderType::Pixel; }
};

class PixelShader {
public:
    static PixelShader make(Array<u8>&& binary);

    inline Slice<const u8> binary() const { return m_interface->binary(); }
    inline ShaderType type() const { return m_interface->type(); }

private:
    PixelShader(Shared<PixelShaderInterface, SMode::Atomic>&& interface) : m_interface(core::move(interface)) { }

    Shared<PixelShaderInterface, SMode::Atomic> m_interface;
};

GPU_NAMESPACE_END