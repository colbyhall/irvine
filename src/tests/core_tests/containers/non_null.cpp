// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/containers/non_null.hpp>

TEST_CASE("NonNull's can reference variables") {
	int x = 45;
	NonNull<int> x_ptr = &x;
	CHECK(*x_ptr == 45);

	NonNull<void> v_ptr = &x;
	CHECK((void*)v_ptr == &x);
}
