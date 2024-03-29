// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.h>
#include <core/containers/slice.h>

TEST_CASE("Slice's reference arrays") {
	int buffer[] = { 1, 2, 3, 4 };
	Slice<int> slice = { buffer, 4 };

	REQUIRE(slice.len() == 4);

	SUBCASE("accessing values") {
		CHECK(slice[0] == 1);
		CHECK(slice[1] == 2);
		CHECK(slice[2] == 3);
		CHECK(slice[3] == 4);
	}

	SUBCASE("shrinking") {
		REQUIRE(slice.shrink(2) == 2);

		CHECK(slice[0] == 1);
		CHECK(slice[1] == 2);
	}
}
