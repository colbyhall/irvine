// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>

TEST_MAIN()

#include <game/ecs/world.hpp>

SUPPRESS_WARNING_PUSH
SUPPRESS_WARNINGS

using namespace game;

class TestComponentA final : public Component {
public:
	DECLARE_COMPONENT(TestComponentA);

	explicit TestComponentA() = default;
	TestComponentA(TestComponentA&& move) noexcept {}
	TestComponentA& operator=(TestComponentA&& m) noexcept {
		return *this;
	}

	StringView a = "Foo";
};

class ABC final : public Component {
public:
	DECLARE_COMPONENT(ABC);

	explicit ABC() = default;
	ABC(ABC&& move) noexcept {}
	ABC& operator=(ABC&& m) noexcept {
		return *this;
	}

	StringView a = "Bar";
};

TEST_CASE("Archetypes hold entities components") {
	auto archetype_id = ArchetypeId();
	archetype_id.with(ComponentId::of<TestComponentA>());
	archetype_id.with(ComponentId::of<ABC>());
	auto archetype = Archetype(archetype_id);

	const auto entity_id = EntityId {
		.generation = 123,
		.index = 1,
	};

	archetype.spawn(entity_id);
	archetype.insert(entity_id, TestComponentA());
	archetype.insert(entity_id, ABC());

	auto a = archetype.get<TestComponentA>(entity_id);
	CHECK(a.is_set());

	auto b = archetype.get<ABC>(entity_id);
	CHECK(b.is_set());
}

TEST_CASE("Worlds hold entities and components") {
	auto world = World();
	auto entity = world.spawn();
	entity
		.insert(ABC())
		.insert(TestComponentA());

	auto a = entity.get<TestComponentA>();
	CHECK(a.is_set());

	auto b = entity.get<ABC>();
	CHECK(b.is_set());
}

SUPPRESS_WARNING_POP
