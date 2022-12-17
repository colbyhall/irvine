// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

// Begin the res namespace
#define RES_NAMESPACE_BEGIN																    \
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace res {

// End the res namespace
#define RES_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP
