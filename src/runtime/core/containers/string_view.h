// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/hash.h>
#include <core/containers/slice.h>
#include <core/containers/option.h>

CORE_NAMESPACE_BEGIN

template<typename T>
constexpr usize strlen(const T* string) {
    if (*string == 0) return 0;
    string += 1;
    return 1 + strlen(string);
}

using Char = u32;
constexpr Char EOS = 0;
constexpr Char EOL = '\n';
constexpr Char UTF8_BOM = 0xfeff;

constexpr u32 utf8_accept = 0;
constexpr u32 utf8_reject = 12;

u32 utf8_decode(u32* state, u32* code_p, u32 byte);
i32 utf8_encode(u32 c, void* dest, u32* errors);

class CharsIterator {
public:
    inline explicit CharsIterator(Slice<char const> string) :
		m_string(string),
		m_index(0),
		m_decoder_state(0),
		m_codepoint(0) {}

    inline explicit operator bool() const { return should_continue(); }
    inline CharsIterator& operator++() { next(); return *this; }
    inline Char operator*() const { return get(); }

private:
    bool should_continue() const;
    void next();
    Char get() const;

    Slice<char const> m_string;
    usize m_index;
    u32 m_decoder_state;
    Char m_codepoint;
};

class StringView {
public:
    inline constexpr StringView() {}
    inline constexpr StringView(Slice<char const> bytes) :
		m_bytes(bytes) {}
    inline constexpr StringView(const char* ptr) :
		m_bytes({ ptr, strlen(ptr) }) {}

    inline operator Slice<char const>() const { return m_bytes; }
    inline const char* operator*() const { return &m_bytes[0]; }

    inline usize len() const { return m_bytes.len(); }
    inline CharsIterator chars() const { return CharsIterator(m_bytes); }
    bool operator==(const StringView& right) const;
    bool operator!=(const StringView& right) const;

	Option<StringView> rsplit(Char c);

private:
    Slice<char const> m_bytes;
};

CORE_NAMESPACE_END

// Export Char, StringView, and CharsIterator out of core namespace
using core::Char;
using core::StringView;
using core::CharsIterator;

void hash(Hasher& hasher, const StringView& view);
