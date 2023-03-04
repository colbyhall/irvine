// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/string.h>

CORE_NAMESPACE_BEGIN

String String::from(Array<char>&& bytes) {
	String string;
	string.m_bytes = core::forward<Array<char>>(bytes);
	if (string.m_bytes.len() > 0 && string.m_bytes[string.m_bytes.len() - 1] != 0) {
		string.m_bytes.push(0);
	}
	return string;
}

String String::from(StringView view) {
	String string;
	string.m_bytes = Array<char>(view);
	string.m_bytes.push(0);
	return string;
}

String String::from(WStringView string) {
	// FIXME: Do proper utf16 decode
	String ret;
	ret.reserve(string.len());
	for (wchar_t c : string) ret.push((Char)c);
	return ret;
}

String::operator Slice<char const>() const {
	Slice<char const> result = m_bytes;
	if (m_bytes.len() > 0) {
		result.shrink(1);
	}
	return result;
}

String::operator StringView() const {
	Slice<char const> bytes = m_bytes;

	// Exclude the null terminator
	bytes.shrink(m_bytes.len() - len());

	return StringView(bytes);
}

String& String::push(Char c) {
	// Encode the utf32 character to an utf8 multi width character
	u8 local[4] = {};
	u32 error;
	const usize char_len = utf8_encode(c, local, &error);
	ASSERT(error != utf8_reject);

	// Preallocate enough space to add the bytes
	const usize slag = m_bytes.cap() - m_bytes.len();
	if (slag < char_len) m_bytes.reserve(char_len + 1);

	// Add the null terminator if the string len was 0
	if (m_bytes.len() == 0) m_bytes.push(0);

	// SPEED: Could be made faster if used core::copy
	for (usize i = 0; i < char_len; ++i) m_bytes.insert(m_bytes.len() - 1, local[i]);

	return *this;
}

String& String::push(StringView string) {
	// Preallocate enough space for the entire string to reduce allocations
	const usize slag = m_bytes.cap() - m_bytes.len();
	if (slag < string.len()) m_bytes.reserve(string.len());

	// Add the null terminator if the string len was 0
	if (m_bytes.len() == 0) m_bytes.push(0);

	// SPEED: could be made faster if used core::copy
	for (u32 i = 0; i < string.len(); ++i) m_bytes.insert(m_bytes.len() - 1, (*string)[i]);

	return *this;
}

CORE_NAMESPACE_END
