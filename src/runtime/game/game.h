// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

// Begin the game namespace
#define GAME_NAMESPACE_BEGIN																    \
	SUPPRESS_WARNING_PUSH																	\
	SUPPRESS_WARNINGS																		\
	namespace game {

// End the game namespace
#define GAME_NAMESPACE_END																	\
	}																						\
	SUPPRESS_WARNING_POP
