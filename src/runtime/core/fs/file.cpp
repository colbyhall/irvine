// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/wstring.h>
#include <core/fs/file.h>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
#endif

CORE_NAMESPACE_BEGIN

Result<File, FileOpenError> File::open(PathView path, FileFlags flags) {
	const bool read = (flags & FF_Read) != 0;
	const bool write = (flags & FF_Write) != 0;
	const bool create = (flags & FF_Create) != 0;
	ASSERT(read || write);

	DWORD access = 0;
	if (read) access |= GENERIC_READ;
	if (write) access |= GENERIC_WRITE;

	DWORD creation = OPEN_EXISTING;
	if (create) creation = OPEN_ALWAYS;

	WString wpath;
	wpath.reserve(path.len() + 16);
	// TODO: Prepend this to allow paths past MAX_PATH
	// path.push(L"\\\\?\\");
	wpath.push(path);

	void* handle = CreateFileW(
		wpath.ptr(),
		access,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		creation,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	const DWORD err = GetLastError();
	ASSERT(err == 0, "TODO File open error handling");

	return File{ handle, flags };
}

usize File::size() const {
	DWORD high;
	const DWORD low = GetFileSize(m_handle, &high);
	return (static_cast<usize>(high) << 32) | static_cast<usize>(low);
}

usize File::seek(Seek method, isize distance) {
	auto win32_method = static_cast<DWORD>(method);

	LARGE_INTEGER win32_distance;
	win32_distance.QuadPart = distance;
	LARGE_INTEGER new_cursor;
	const bool ok = SetFilePointerEx(
		m_handle,
		win32_distance,
		&new_cursor,
		win32_method
	);
	ASSERT(ok);

	m_cursor = static_cast<usize>(new_cursor.QuadPart);
	return m_cursor;
}

void File::set_eof() {
	ASSERT((m_flags & FF_Write) != 0, "Can only write to file that has been open with FF_Write");
	const bool ok = SetEndOfFile(m_handle);
	ASSERT(ok);
}

usize File::read(Slice<u8> buffer) {
	ASSERT((m_flags & FF_Read) != 0, "Can only read a file that has been open with FF_Read");

	DWORD amount_read;
	const bool ok = ReadFile(
		m_handle,
		buffer.begin(),
		(DWORD)buffer.len(),
		&amount_read,
		nullptr
	);
	ASSERT(ok);

	m_cursor += amount_read;

	return amount_read;
}

void File::write(Slice<const u8> buffer) {
	ASSERT((m_flags & FF_Write) != 0, "Can only write to file that has been open with FF_Write");

	const bool ok = WriteFile(
		m_handle,
		buffer.cbegin(),
		(DWORD)buffer.len(),
		nullptr,
		nullptr
	);
	ASSERT(ok);

	m_cursor += buffer.len();
}

File::~File() {
	if (m_handle) {
		const bool ok = CloseHandle((HANDLE)m_handle) > 0;
		ASSERT(ok);
		m_handle = nullptr;
	}
}

Result<String, FileOpenError> read_to_string(PathView path) {
	// Open the file to read
	auto result = File::open(path, FF_Read);
	if (!result.is_ok()) return result.unwrap_err();
	File file = result.unwrap();

	// Allocate a buffer that is the size of the file
	const auto size = file.size();
	Array<char> bytes;
	bytes.reserve(size + 1);
	bytes.set_len(size);

	// Read the file into the buffer
	file.read({ (u8*)bytes.begin(), bytes.len() });

	// Null terminate
	bytes.push(0);

	return String::from(core::move(bytes));
}

CORE_NAMESPACE_END
