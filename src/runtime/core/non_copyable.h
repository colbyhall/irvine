// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

CORE_NAMESPACE_BEGIN

/// Class that makes another class non-copyable. Usage: Inherit from NonCopyable.
class NonCopyable {
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
	NonCopyable(NonCopyable&&) {}
	void operator=(NonCopyable&&) {}
};

CORE_NAMESPACE_END

// Export NonCopyable out of core namespace
using core::NonCopyable;
