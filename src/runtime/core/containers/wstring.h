// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/wstring_view.h>
#include <core/containers/array.h>

CORE_NAMESPACE_BEGIN

wchar utf32_to_utf16(Codepoint c);

class WString {
public:
    inline constexpr WString() : m_chars() {}

    static WString from(WStringView view);
    static WString from(StringView string);

    operator Slice<wchar const>() const;
    operator WStringView() const;

    FORCE_INLINE wchar* ptr() { return m_chars.begin(); }
    FORCE_INLINE wchar const* ptr() const { return m_chars.cbegin(); }
    FORCE_INLINE const wchar* operator*() const { return m_chars.cbegin(); }

    FORCE_INLINE usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
    FORCE_INLINE usize cap() const { return m_chars.cap(); }
    FORCE_INLINE void set_len(usize len) {
        m_chars.set_len(len + 1);
        m_chars[len] = 0;
    }

    FORCE_INLINE wchar* begin() { return m_chars.begin(); }
    FORCE_INLINE wchar* end() { return m_chars.end(); }
    FORCE_INLINE const wchar* cbegin() const { return m_chars.cbegin(); }
    FORCE_INLINE const wchar* cend() const { return m_chars.cend(); }

    FORCE_INLINE wchar& operator[](usize index) { return m_chars[index]; }
    FORCE_INLINE wchar operator[](usize index) const { return m_chars[index]; }

    FORCE_INLINE void reserve(usize amount) { return m_chars.reserve(amount); }
    WString& push(wchar w);
    WString& push(WStringView string);
    WString& push(StringView string);

private:
    Array<wchar> m_chars;
};

CORE_NAMESPACE_END

// Export WString out of core namespace
using core::WString;
