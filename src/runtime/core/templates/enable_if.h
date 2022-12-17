// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

CORE_NAMESPACE_BEGIN

// https://en.cppreference.com/w/cpp/types/enable_if
template <bool B, typename T = void>
class EnableIf;

template <typename T>
struct EnableIf<true, T> {
	using Type = T;
};

template <typename T>
class EnableIf<false, T> { };

template <bool B, typename T = void>
using EnabledIf = typename EnableIf<B, T>::Type;

CORE_NAMESPACE_END
