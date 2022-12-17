// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

// Begin the gui namespace
#define GUI_NAMESPACE_BEGIN																    \
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace gui {

// End the gui namespace
#define GUI_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP
