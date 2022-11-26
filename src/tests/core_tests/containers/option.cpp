// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/containers/option.hpp>

TEST_CASE("Option's manage an optional value, i.e a value that may or may not be set.") {
	Option<int> x = nullptr;
	CHECK(!x.is_set());
	x = 45;
	REQUIRE(x.is_set());
	CHECK(x.unwrap() == 45);
	// CHECK(x.is_set());
}
