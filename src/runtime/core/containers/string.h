// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.h>
#include <core/containers/string_view.h>
#include <core/containers/wstring_view.h>

CORE_NAMESPACE_BEGIN

class String {
public:
    String() = default;
    static String from(Array<char>&& bytes);
    static String from(StringView view);
    static String from(WStringView string);

    operator Slice<char const>() const;
    operator StringView() const;
    FORCE_INLINE char* operator*() { return &m_bytes[0]; }
    FORCE_INLINE const char* operator*() const { return &m_bytes[0]; }

    FORCE_INLINE CodepointsIterator codepoints() const { return CodepointsIterator(m_bytes); }
    FORCE_INLINE usize len() const { return m_bytes.len() > 0 ? m_bytes.len() - 1 : 0; }
    FORCE_INLINE usize cap() const { return m_bytes.cap(); }
    FORCE_INLINE bool operator==(StringView rhs) const { StringView view = *this; return view == rhs; }
    FORCE_INLINE bool operator!=(StringView rhs) const { StringView view = *this; return view != rhs; }

    FORCE_INLINE void reserve(usize amount) { m_bytes.reserve(amount + 1); }
    String& push(Codepoint c);
    String& push(StringView string);

    FORCE_INLINE void set_len(usize len) {
        m_bytes.set_len(len + 1);
        m_bytes[len] = 0;
    }

private:
    Array<char> m_bytes;
};

CORE_NAMESPACE_END

// Export String out of core namespace
using core::String;
