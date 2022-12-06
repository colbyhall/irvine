// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/memory.hpp>

TEST_CASE("count ones") {
	const int x = 0xFF00FF00; // has 16 bits set to one
	CHECK(core::mem::count_ones(x) == 16);
}
