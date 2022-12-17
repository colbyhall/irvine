// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.h>
#include <core/containers/unique.h>

struct Base {
	int a;

	Base(int _a) : a(_a) {}
};

struct Derived : public Base {
	int b;

	Derived(int _a, int _b) : Base(_a), b(_b) {}
};

TEST_CASE("Unique Pointers") {
	Unique<Base> unique_ptr = make_unique<Derived>(1, 2);
	CHECK(unique_ptr->a == 1);
}
