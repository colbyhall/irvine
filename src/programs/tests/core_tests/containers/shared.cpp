// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/containers/shared.hpp>

struct Base {
	int a;

	Base(int _a) : a(_a) {}
};

struct Derived : public Base {
	int b;

	Derived(int _a, int _b) : Base(_a), b(_b) {}
};

TEST_CASE("Shared Pointers") {
	Shared<Base, SMode::Atomic> shared = make_shared<Derived, SMode::Atomic>(1, 2);
	CHECK(shared->a == 1);
	CHECK(shared.strong() == 1);
	auto other = shared.clone();
	CHECK(shared->a == 1);
	CHECK(shared.strong() == 2);
}
