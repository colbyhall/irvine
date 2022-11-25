// Copyright Colby Hall. All Rights Reserved.

#pragma once

/// Class that makes another class non-copyable. Usage: Inherit from NonCopyable.
class NonCopyable {
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};
