// Copyright Colby Hall. All Rights Reserved.

#include <Doctest/Doctest.h>
#include <Core/Containers/NonNull.h>

TEST_CASE("NonNull's can reference variables") {
	int x = 45;
	NonNull<int> x_ptr = &x;
	CHECK(*x_ptr == 45);
}
