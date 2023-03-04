// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.h>
#include <core/containers/option.h>

TEST_CASE(
	"Option's manage an optional value"
	", i.e a value that may or may not be set."
) {
	Option<int> x = core::none;
	CHECK(!x.is_set());
	x = 45;
	REQUIRE(x.is_set());
	CHECK(x.unwrap() == 45);
}
