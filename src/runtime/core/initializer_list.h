// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

SUPPRESS_WARNINGS_STD_BEGIN
#include <initializer_list>
SUPPRESS_WARNINGS_STD_END

CORE_NAMESPACE_BEGIN

template <typename T>
using InitializerList = std::initializer_list<T>;

CORE_NAMESPACE_END
