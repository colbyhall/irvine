// Copyright Colby Hall. All Rights Reserved.

#include <core/core.hpp>

#ifdef PLATFORM_WINDOWS

#include <core/platform/windows.hpp>
#include <cstdio>

bool _assert_failed(bool must_crash, const char* expression, const char* message, const char* file, u32 line) {
	char buffer[1024];
	buffer[0] = 0;

	static const char* ensure_usage = "Press Yes to breakpoint. No to continue.";
	if (message) {
		if (must_crash) {
			sprintf_s(buffer, "%s\n\n(%s) evaluated to false\n\n%s Line %d", message, expression, file, line);
		} else {
			sprintf_s(buffer, "%s\n\n(%s) evaluated to false\n\n%s Line %d\n\n%s", message, expression, file, line, ensure_usage);
		}
	} else {
		if (must_crash) {
			sprintf_s(buffer, "(%s) evaluated to false\n\n%s Line %d", expression, file, line);
		}
		else {
			sprintf_s(buffer, "(%s) evaluated to false\n\n%s Line %d\n\n%s", expression, file, line, ensure_usage);
		}
	}
	const char* title = must_crash ? "Application Assert" : "Application Ensure";
	const int result = MessageBoxA(NULL, buffer, title, UINT(must_crash ? (MB_OK | MB_ICONERROR) : (MB_YESNO | MB_ICONWARNING)));
	return result != IDNO;
}

#else
	#error Unimplemented Assert
#endif
