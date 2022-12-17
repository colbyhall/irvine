// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string.h>
#include <core/fs/path_view.h>

CORE_NAMESPACE_BEGIN

class Path {
public:
    FORCE_INLINE constexpr Path() : m_string() {}
    explicit inline Path(PathView view) : m_string(String::from(view)) {}
    FORCE_INLINE Path(String&& string) noexcept : m_string(core::forward<String>(string)) {}

    FORCE_INLINE static Path from(StringView string) { return Path(String::from(string)); }
    FORCE_INLINE static Path from(WStringView wstring) { return Path(String::from(wstring)); }

    FORCE_INLINE operator StringView() const { return m_string; }
    FORCE_INLINE operator PathView() const { return PathView(m_string); }
    FORCE_INLINE const char* operator*() const { return *m_string; }

    FORCE_INLINE usize len() const { return m_string.len(); }
    FORCE_INLINE CodepointsIterator codepoints() const { return CodepointsIterator(m_string); }

private:
    String m_string;
};

CORE_NAMESPACE_END

using core::Path;
