// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/wstring.h>
#include <core/fs/dir.hpp>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
#endif


CORE_NAMESPACE_BEGIN

static void read_directory_impl(PathView path, bool recursive, ReadDirFunction& function) {
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

			if (recursive) read_directory_impl(new_path, recursive, function);

			item.path = core::move(new_path);
		}
		else {
			item.type = DirectoryItemType::File;
			item.path = core::move(new_path);

			item.meta_data.read_only = (find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
			item.meta_data.size = find_data.nFileSizeLow;
		}

		if (!function(item)) break;
	} while (FindNextFileW(find_handle, &find_data));

	FindClose(find_handle);
}

void read_dir(PathView path, ReadDirFunction function) {
	read_directory_impl(path, false, function);
}

void read_dir_recursive(PathView path, ReadDirFunction function) {
	read_directory_impl(path, true, function);
}

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

CORE_NAMESPACE_END
