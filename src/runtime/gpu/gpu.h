// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

// Begin the gpu namespace
#define GPU_NAMESPACE_BEGIN																    \
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace gpu {

// End the gpu namespace
#define GPU_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP
