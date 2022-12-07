// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>
#include <core/math/vec2.hpp>
#include <core/math/mat3.hpp>

TEST_CASE("Vec2 math operations") {
	auto a = Vec2f32{ 10.f, 15.f };
	a += 10.f;
	CHECK(a.x == 20.f);
	CHECK(a.y == 25.f);
	a -= 10.f;
	CHECK(a.x == 10.f);
	CHECK(a.y == 15.f);
	a *= 10.f;
	CHECK(a.x == 100.f);
	CHECK(a.y == 150.f);
	a /= 10.f;
	CHECK(a.x == 10.f);
	CHECK(a.y == 15.f);

	auto one = Vec2f32{ 1, 0 };
	CHECK(one.len_sq() == 1.f);
	CHECK(one.len() == 1.f);
}
