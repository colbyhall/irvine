// Copyright Colby Hall. All Rights Reserved.

#include <gui/gui.hpp>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.hpp>
#endif

GUI_NAMESPACE_BEGIN

Context& Context::the() {
	static Context instance;
	return instance;
}

Context::Context() {

}

GUI_NAMESPACE_END
