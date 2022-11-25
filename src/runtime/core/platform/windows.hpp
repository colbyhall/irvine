// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

SUPPRESS_WARNING_PUSH
MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)

#define WIN32_LEAN_AND_MEAN
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

SUPPRESS_WARNING_POP
