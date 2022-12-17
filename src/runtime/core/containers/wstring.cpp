// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/wstring.h>

CORE_NAMESPACE_BEGIN

wchar utf32_to_utf16(Codepoint c) {
	u32 h;
	u32 l;

	if (c < 0x10000) {
		h = 0;
		l = c;
		return (wchar)c;
	}
	Codepoint t = c - 0x10000;
	h = (((t<<12)>>22) + 0xD800);
	l = (((t<<22)>>22) + 0xDC00);
	Codepoint ret = ((h<<16) | ( l & 0x0000FFFF));
	return (wchar)ret;
}

WString WString::from(WStringView view) {
	WString string;
	string.m_chars = Array<wchar>(view);
	return string;
}

WString WString::from(StringView string) {
	WString result;
	result.reserve(string.len());

	for (auto codepoints = string.codepoints(); codepoints; ++codepoints) {
		const Codepoint c = *codepoints;
		result.push(utf32_to_utf16(c));
	}

	return result;
}

WString::operator Slice<wchar const>() const {
	Slice<wchar const> result = m_chars;
	if (m_chars.len() > 0) {
		result.shrink(1);
	}
	return result;
}

WString::operator WStringView() const {
	Slice<wchar const> bytes = m_chars;

	// Exclude the null terminator
	bytes.shrink(m_chars.len() - len());

	return WStringView(bytes);
}

WString& WString::push(wchar w) {
	const auto start_len = m_chars.len();

	if (start_len == 0) {
		m_chars.push(w);
		m_chars.push(0);
	} else {
		m_chars.insert(start_len - 1, w);
	}

	return *this;
}

WString& WString::push(WStringView string) {
	const usize slag = m_chars.cap() - m_chars.len();
	if (slag < string.len()) m_chars.reserve(string.len());

	if (m_chars.len() == 0) m_chars.push(0);
	for (wchar w : string) m_chars.insert(m_chars.len() - 1, w);

	return *this;
}

WString& WString::push(StringView string) {
	const usize slag = m_chars.cap() - m_chars.len();
	if (slag < string.len()) m_chars.reserve(string.len());

	for (auto codepoints = string.codepoints(); codepoints; ++codepoints) {
		push(utf32_to_utf16(*codepoints));
	}

	return *this;
}

CORE_NAMESPACE_END
