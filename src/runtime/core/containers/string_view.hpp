// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/slice.hpp>

CORE_NAMESPACE_BEGIN

template<typename T>
constexpr usize strlen(const T* string) {
    if (*string == 0) return 0;
    string += 1;
    return 1 + strlen(string);
}

using Codepoint = u32;
constexpr Codepoint EOS = 0;
constexpr Codepoint EOL = '\n';
constexpr Codepoint UTF8_BOM = 0xfeff;

constexpr u32 utf8_accept = 0;
constexpr u32 utf8_reject = 12;

u32 utf8_decode(u32* state, u32* code_p, u32 byte);
i32 utf8_encode(u32 c, void* dest, u32* errors);

class CodepointsIterator {
public:
    FORCE_INLINE explicit CodepointsIterator(Slice<char const> string) : m_string(string), m_index(0), m_decoder_state(0), m_codepoint(0) {}

    FORCE_INLINE explicit operator bool() const { return should_continue(); }
    FORCE_INLINE CodepointsIterator& operator++() { next(); return *this; }
    FORCE_INLINE Codepoint operator*() const { return get(); }

private:
    bool should_continue() const;
    void next();
    Codepoint get() const;

    Slice<char const> m_string;
    usize m_index;
    u32 m_decoder_state;
    Codepoint m_codepoint;
};

class StringView {
public:
    FORCE_INLINE constexpr StringView() : m_bytes() {}
    FORCE_INLINE constexpr StringView(Slice<char const> bytes) : m_bytes(bytes) {}
    FORCE_INLINE constexpr StringView(const char* ptr) : m_bytes({ ptr, strlen(ptr) }) {}

    FORCE_INLINE operator Slice<char const>() const { return m_bytes; }
    FORCE_INLINE const char* operator*() const { return &m_bytes[0]; }

    FORCE_INLINE usize len() const { return m_bytes.len(); }
    FORCE_INLINE CodepointsIterator codepoints() const { return CodepointsIterator(m_bytes); }
    bool operator==(const StringView& right) const;
    FORCE_INLINE bool operator!=(const StringView& right) const { return !(*this == right); }

private:
    Slice<char const> m_bytes;
};

CORE_NAMESPACE_END

// Export Codepoint, StringView, and CodepointsIterator out of core namespace
using core::Codepoint;
using core::StringView;
using core::CodepointsIterator;