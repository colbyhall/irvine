// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string.h>
#include <core/fs/path_view.h>

CORE_NAMESPACE_BEGIN

class Path {
public:
	inline constexpr Path() : m_string() {}
	explicit inline Path(PathView view) : m_string(String::from(view)) {}
	inline Path(String&& string) noexcept
		: m_string(core::forward<String>(string)) {}

	inline static Path from(StringView string) { return Path(String::from(string)); }
	inline static Path from(WStringView wstring) { return Path(String::from(wstring)); }

	inline operator StringView() const { return m_string; }
	inline operator PathView() const { return PathView(m_string); }
	inline const char* operator*() const { return *m_string; }

	inline usize len() const { return m_string.len(); }
	inline CharsIterator chars() const { return CharsIterator(m_string); }

private:
	String m_string;
};

CORE_NAMESPACE_END

using core::Path;
