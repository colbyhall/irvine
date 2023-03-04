// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/wstring_view.h>
#include <core/containers/array.h>

CORE_NAMESPACE_BEGIN

wchar utf32_to_utf16(Char c);

class WString {
public:
	inline constexpr WString() : m_chars() {}

	static WString from(WStringView view);
	static WString from(StringView string);

	operator Slice<wchar const>() const;
	operator WStringView() const;

	inline wchar* ptr() { return m_chars.begin(); }
	inline wchar const* ptr() const { return m_chars.cbegin(); }
	inline const wchar* operator*() const { return m_chars.cbegin(); }

	inline usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
	inline usize cap() const { return m_chars.cap(); }
	inline void set_len(usize len) {
		m_chars.set_len(len + 1);
		m_chars[len] = 0;
	}

	inline wchar* begin() { return m_chars.begin(); }
	inline wchar* end() { return m_chars.end(); }
	inline const wchar* cbegin() const { return m_chars.cbegin(); }
	inline const wchar* cend() const { return m_chars.cend(); }

	inline wchar& operator[](usize index) { return m_chars[index]; }
	inline wchar operator[](usize index) const { return m_chars[index]; }

	inline void reserve(usize amount) { return m_chars.reserve(amount); }
	WString& push(wchar w);
	WString& push(WStringView string);
	WString& push(StringView string);

private:
	Array<wchar> m_chars;
};

CORE_NAMESPACE_END

// Export WString out of core namespace
using core::WString;
