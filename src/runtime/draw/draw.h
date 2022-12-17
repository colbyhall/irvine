// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

// Begin the draw namespace
#define DRAW_NAMESPACE_BEGIN																\
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace draw {

// End the draw namespace
#define DRAW_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP
