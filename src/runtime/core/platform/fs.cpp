// Copyright Colby Hall. All Rights Reserved.

#include <core/platform/fs.hpp>
#include <core/containers/wstring.hpp>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.hpp>
#endif

CORE_NAMESPACE_BEGIN

#ifdef PLATFORM_WINDOWS

Path cwd() {
	// Query the length of the path
	const auto len = (usize)GetCurrentDirectoryW(0, nullptr);
	ASSERT(len > 0);

	// Create a wide string buffer to get the cwd path
	WString buffer;
	buffer.reserve(len);
	GetCurrentDirectoryW((DWORD)buffer.cap(), buffer.ptr());
	buffer.set_len(len);

	// Copy all data to String
	return { String::from(buffer) };
}

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
static void read_directory_impl(PathView path, bool recursive, Array<DirectoryItem>& items) {
	WString wpath;
	wpath.reserve(path.len() + 16);
	// TODO: Prepend this to allow paths past MAX_PATH
	// path.push(L"\\\\?\\");
	wpath.push(path);
	const auto wpath_len = wpath.len();
	wpath.push(L"\\*"); // TODO: Check if slash is there first

	WIN32_FIND_DATAW find_data;
	HANDLE find_handle = FindFirstFileW(wpath.ptr(), &find_data);

	wpath.set_len(wpath_len);

	if (find_handle == INVALID_HANDLE_VALUE) {
		//const auto error = GetLastError();
		ASSERT(true, "Check error");
	}

	do {
		// Check to see if cFileName is "." or ".."
		bool invalid = find_data.cFileName[0] == L'.' && find_data.cFileName[1] == 0;
		invalid |= find_data.cFileName[0] == L'.' && find_data.cFileName[1] == L'.' && find_data.cFileName[2] == 0;
		if (invalid) continue;

		wpath.set_len(wpath_len);
		DirectoryItem item;

		// Convert all the FILETIME to u64
		FILETIME creation_time = find_data.ftCreationTime;
		item.meta_data.creation_time = (u64)creation_time.dwHighDateTime << 32 | creation_time.dwLowDateTime;
		FILETIME last_access_time = find_data.ftLastAccessTime;
		item.meta_data.last_access_time = (u64)last_access_time.dwHighDateTime << 32 | last_access_time.dwLowDateTime;
		FILETIME last_write_time = find_data.ftLastWriteTime;
		item.meta_data.last_write_time = (u64)last_write_time.dwHighDateTime << 32 | last_write_time.dwLowDateTime;

		// Add a slash if one is not at the end
		const wchar_t last = wpath[wpath.len() - 1];
		if (last != L'\\' && last != L'/') wpath.push("\\");

		// Add the new filename to the end and convert to path
		for (int i = 0; i < MAX_PATH; ++i) {
			const WCHAR w = find_data.cFileName[i];
			if (w == 0) break;
			wpath.push(w);
		}
		auto new_path = Path::from(wpath);

		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			item.type = DirectoryItemType::Directory;

			if (recursive) read_directory_impl(new_path, recursive, items);

			item.path = core::move(new_path);
		}
		else {
			item.type = DirectoryItemType::File;
			item.path = core::move(new_path);

			item.meta_data.read_only = (find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
			item.meta_data.size = find_data.nFileSizeLow;
		}

		items.push(core::move(item));
	} while (FindNextFileW(find_handle, &find_data));

	FindClose(find_handle);
}

Array<DirectoryItem> read_directory(PathView path, bool recursive) {
	Array<DirectoryItem> items;
	read_directory_impl(path, recursive, items);
	return items;
}

#endif

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
