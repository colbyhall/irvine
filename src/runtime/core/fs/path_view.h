// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/string_view.h>

CORE_NAMESPACE_BEGIN

class PathView {
public:
	inline constexpr PathView() {}
	inline constexpr PathView(Slice<char const> bytes) : m_string(bytes) {}
	inline constexpr PathView(const char* ptr)
		: m_string({ ptr, core::strlen(ptr) }) {}

	inline operator StringView() const { return m_string; }
	inline const char* operator*() const { return *m_string; }

	inline usize len() const { return m_string.len(); }
	inline CharsIterator chars() const { return CharsIterator(m_string); }

	Option<StringView> extension() const;

private:
	StringView m_string;
};

CORE_NAMESPACE_END

using core::PathView;
