// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string_view.h>

CORE_NAMESPACE_BEGIN

class WStringView {
public:
    FORCE_INLINE constexpr WStringView() : m_chars() {}
    FORCE_INLINE constexpr WStringView(Slice<wchar_t const> bytes) : m_chars(bytes) {}
    FORCE_INLINE constexpr WStringView(const wchar_t* ptr) : m_chars({ ptr, core::strlen(ptr) }) {}

    FORCE_INLINE operator Slice<wchar_t const>() const { return m_chars; }

    FORCE_INLINE usize len() const { return m_chars.len(); }
    FORCE_INLINE const wchar_t* operator*() const { return &m_chars[0]; }

    FORCE_INLINE const wchar_t* begin() const { return m_chars.cbegin(); }
    FORCE_INLINE const wchar_t* end() const { return m_chars.cend(); }
    FORCE_INLINE const wchar_t* cbegin() const { return m_chars.cbegin(); }
    FORCE_INLINE const wchar_t* cend() const { return m_chars.cend(); }

    FORCE_INLINE wchar_t operator[](usize index) const { return m_chars[index]; }

private:
    Slice<wchar_t const> m_chars;
};

CORE_NAMESPACE_END

// Export WStringView out of core namespace
using core::WStringView;
