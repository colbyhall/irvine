// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.h>

TEST_MAIN()

#include <core/containers/string_view.h>
#include <core/containers/hash_map.h>

TEST_CASE("hash") {
	HashMap<StringView, i32> map;
	map.insert("Hello World", 42);

	const i32& value = map.find("Hello World").unwrap();
	CHECK(value == 42);
}
