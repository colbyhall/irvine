// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/mat4.h>
#include <core/math/vec3.h>
#include <core/math/quat.h>

#include <game/ecs/component.h>
#include <game/ecs/entity.h>

GAME_NAMESPACE_BEGIN

class Transform final : public Component {
public:
	DECLARE_COMPONENT(Transform);

private:
	Vec3f32 m_position;
	Quatf32 m_rotation;
	Vec3f32 m_scale;
};

class Link final : public Component {
public:
	DECLARE_COMPONENT(Link);

private:
	Option<EntityId> m_parent;
	Array<EntityId> m_children;

	Mat4f32 m_local_to_world = Mat4f32::identity;
};

GAME_NAMESPACE_END
