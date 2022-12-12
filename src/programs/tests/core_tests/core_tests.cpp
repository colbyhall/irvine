// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>

TEST_MAIN()

#include <core/containers/string_view.hpp>
#include <core/containers/hash_map.hpp>

TEST_CASE("hash") {
	HashMap<StringView, i32> map;
	map.insert("Hello World", 42);

	const i32& value = map.find("Hello World").unwrap();
	CHECK(value == 42);
}
