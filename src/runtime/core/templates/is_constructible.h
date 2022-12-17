// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

CORE_NAMESPACE_BEGIN

template <typename T, typename... A>
inline constexpr bool is_constructible = __is_constructible(T, A...);

CORE_NAMESPACE_END
