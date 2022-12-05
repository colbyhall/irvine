// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

// Begin the gui namespace
#define GUI_NAMESPACE_BEGIN																    \
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace gui {

// End the gui namespace
#define GUI_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP

GUI_NAMESPACE_BEGIN

class Context {
public:
	static Context& the();

	Context();
private:
};

GUI_NAMESPACE_END