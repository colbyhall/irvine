// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>

CORE_NAMESPACE_BEGIN

/// Class that makes another class non-copyable. Usage: Inherit from NonCopyable.
class NonCopyable {
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

CORE_NAMESPACE_END

// Export NonCopyable out of core namespace
using core::NonCopyable;