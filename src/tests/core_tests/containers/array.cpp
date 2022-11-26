// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/containers/array.hpp>

TEST_CASE("Array's are the std::vector equivilent") {
	SUBCASE("Pushing int to Array") {
		Array<int> arr;
		arr.push(1);
		REQUIRE(arr.len() == 1);
		CHECK(arr[0] == 1);
	}

	SUBCASE("Inserting int to beginning of Array") {
		Array<int> arr;
		arr.push(1);
		REQUIRE(arr.len() == 1);
		CHECK(arr[0] == 1);
		arr.insert(0, 2);
		REQUIRE(arr.len() == 2);
		CHECK(arr[0] == 2);
		CHECK(arr[1] == 1);
	}

	SUBCASE("Removing value from Array") {
		Array<int> arr;
		arr.push(1);
		REQUIRE(arr.len() == 1);
		CHECK(arr[0] == 1);
		const int removed = arr.remove(0);
		CHECK(arr.is_empty());
		CHECK(removed == 1);
	}
}
