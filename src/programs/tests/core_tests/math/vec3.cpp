// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/math/vec3.hpp>

TEST_CASE("Vec3 math operations") {
	auto a = Vec3f32{ 10.f, 15.f, 0.f };
	a += 10.f;
	CHECK(a.x == 20.f);
	CHECK(a.y == 25.f);
	CHECK(a.z == 10.f);
	a -= 10.f;
	CHECK(a.x == 10.f);
	CHECK(a.y == 15.f);
	CHECK(a.z == 0.f);
	a *= 10.f;
	CHECK(a.x == 100.f);
	CHECK(a.y == 150.f);
	CHECK(a.z == 0.f);
	a /= 10.f;
	CHECK(a.x == 10.f);
	CHECK(a.y == 15.f);
	CHECK(a.z == 0.f);

	auto one = Vec3f32{ 1, 0, 0 };
	CHECK(one.len_sq() == 1.f);
	CHECK(one.len() == 1.f);
}
