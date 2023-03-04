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
    inline char* operator*() { return &m_bytes[0]; }
    inline const char* operator*() const { return &m_bytes[0]; }

    inline CharsIterator chars() const { return CharsIterator(m_bytes); }
    inline usize len() const { return m_bytes.len() > 0 ? m_bytes.len() - 1 : 0; }
    inline usize cap() const { return m_bytes.cap(); }
    inline bool operator==(StringView rhs) const { StringView view = *this; return view == rhs; }
    inline bool operator!=(StringView rhs) const { StringView view = *this; return view != rhs; }

    inline void reserve(usize amount) { m_bytes.reserve(amount + 1); }
    String& push(Char c);
    String& push(StringView string);

    inline void set_len(usize len) {
        m_bytes.set_len(len + 1);
        m_bytes[len] = 0;
    }

private:
    Array<char> m_bytes;
};

CORE_NAMESPACE_END

// Export String out of core namespace
using core::String;
