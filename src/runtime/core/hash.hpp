// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/slice.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

class Hasher {
public:
    virtual u64 finish() = 0;
    virtual void write(Slice<u8 const> bytes) = 0;
    virtual ~Hasher() {}
};

class FNV1Hasher : public Hasher {
public:
    constexpr FNV1Hasher() = default;

    static const u64 offset_basic;
    static const u64 prime;

    // Hasher
    u64 finish() override;
    void write(Slice<u8 const> bytes) override;
    // ~Hasher

private:
    u64 m_result = 0;
};

CORE_NAMESPACE_END

using core::Hasher;
using core::FNV1Hasher;

void hash(Hasher& hasher, const u8& value);
void hash(Hasher& hasher, const u16& value);
void hash(Hasher& hasher, const u32& value);
void hash(Hasher& hasher, const u64& value);
void hash(Hasher& hasher, const i8& value);
void hash(Hasher& hasher, const i16& value);
void hash(Hasher& hasher, const i32& value);
void hash(Hasher& hasher, const i64& value);
