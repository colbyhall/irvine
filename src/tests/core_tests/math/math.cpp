// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/math/math.hpp>

TEST_CASE("Min returns the minimum value") {
	CHECK(core::min(4, 5) == 4);
	CHECK(core::min(5, 4) == 4);
}

TEST_CASE("Max returns the maximum value") {
	CHECK(core::max(4, 5) == 5);
	CHECK(core::max(5, 4) == 5);
}

TEST_CASE("Clamp returns the value clamped between a min and max") {
	CHECK(core::clamp(5, 2, 4) == 4);
	CHECK(core::clamp(1, 2, 4) == 2);
	CHECK(core::clamp(3, 2, 4) == 3);
}

TEST_CASE("Abs") {
	CHECK(core::abs(-4) == 4);
	CHECK(core::abs(4) == 4);
}
