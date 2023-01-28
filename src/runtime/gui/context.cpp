// Copyright Colby Hall. All Rights Reserved.

#include <core/containers/option.h>
#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
	#include <core/library.h>
#endif

#include <gui/context.h>

#ifdef PLATFORM_WINDOWS
	typedef enum PROCESS_DPI_AWARENESS {
		PROCESS_DPI_UNAWARE,
		PROCESS_SYSTEM_DPI_AWARE,
		PROCESS_PER_MONITOR_DPI_AWARE
	} PROCESS_DPI_AWARENESS;

	enum MONITOR_DPI_TYPE {
		MDT_EFFECTIVE_DPI,
		MDT_ANGULAR_DPI,
		MDT_RAW_DPI,
		MDT_DEFAULT
	};

	typedef HRESULT(*SetProcessDPIAwareness)(PROCESS_DPI_AWARENESS value);
	typedef HRESULT(*GetDPIForMonitor)(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY);
#endif

GUI_NAMESPACE_BEGIN

void init() {
#ifdef PLATFORM_WINDOWS
	static auto shcore = Library::open("shcore.dll");
	if (shcore) {
		auto& actual = shcore.as_mut().unwrap();
		auto SetProcessDpiAwareness = (SetProcessDPIAwareness)actual.find("SetProcessDpiAwareness");
		if (SetProcessDpiAwareness) SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	}
#endif
}

GUI_NAMESPACE_END
