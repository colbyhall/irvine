// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

CORE_NAMESPACE_BEGIN

// https://en.cppreference.com/w/cpp/types/remove_reference
template<typename T>
struct RemoveReference {
	using Type = T;
};

template <typename T>
struct RemoveReference<T&> {
	using Type = T;
};

template <typename T>
struct RemoveReference<T&&> {
	using Type = T;
};

template <typename T>
using ReferenceRemoved = typename RemoveReference<T>::Type;

// https://en.cppreference.com/w/cpp/types/remove_pointer
template<typename T>
struct RemovePointer {
	using Type = T;
};

template <typename T>
struct RemovePointer<T*> {
	using Type = T;
};

template <typename T>
struct RemovePointer<T**> {
	using Type = T;
};

template <typename T>
using PointerRemoved = typename RemovePointer<T>::Type;

// https://en.cppreference.com/w/cpp/types/remove_cv
template <typename T>
struct RemoveCV {
	using Type = T;
};

template <typename T>
struct RemoveCV<const T> {
	using Type = T;
};

template <typename T>
struct RemoveCV<volatile T> {
	using Type = T;
};

template <typename T>
struct RemoveCV<const volatile T> {
	using Type = T;
};

template <typename T>
using CVRemoved = typename RemoveCV<T>::Type;

CORE_NAMESPACE_END