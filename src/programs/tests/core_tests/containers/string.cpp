// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.h>
#include <core/containers/string.h>

TEST_CASE("Strings are utf8") {
	SUBCASE("StringViews can be created from string literals") {
		StringView a = "Hello World";
		CHECK(a.len() == 11);

		SUBCASE("Strings can be created from StringViews") {
			auto b = String::from(a);
			b.push(". What a lovely day");
			CHECK(b == "Hello World. What a lovely day");
		}
	}
}

TEST_CASE("WStrings are utf16") {
	WStringView a = L"Hello World";
	CHECK(a.len() == 11);
}
