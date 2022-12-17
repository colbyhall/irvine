// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string_view.h>

CORE_NAMESPACE_BEGIN

class PathView {
public:
    FORCE_INLINE constexpr PathView() : m_string() {}
    FORCE_INLINE constexpr PathView(Slice<char const> bytes) : m_string(bytes) {}
    FORCE_INLINE constexpr PathView(const char* ptr) : m_string({ ptr, core::strlen(ptr) }) {}

    FORCE_INLINE operator StringView() const { return m_string; }
    FORCE_INLINE const char* operator*() const { return *m_string; }

    FORCE_INLINE usize len() const { return m_string.len(); }
    FORCE_INLINE CodepointsIterator codepoints() const { return CodepointsIterator(m_string); }

private:
    StringView m_string;
};

CORE_NAMESPACE_END

using core::PathView;
