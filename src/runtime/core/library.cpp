// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/wstring.h>
#include <core/library.h>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
#endif

CORE_NAMESPACE_BEGIN

Option<Library> Library::open(PathView path) {
	WString wpath;
	wpath.reserve(path.len());
	wpath.push(path);

	void* handle = LoadLibraryW(wpath.ptr());
	if (handle != nullptr) return Library { handle };
	return nullptr;
}

Library::~Library() {
	if (m_handle) {
		FreeLibrary((HMODULE)m_handle);
		m_handle = nullptr;
	}
}

void* Library::find(StringView name) {
	return (void*)GetProcAddress((HMODULE)m_handle, *name);
}

CORE_NAMESPACE_END
