// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/slice.hpp>

#include <game/game.hpp>

GAME_NAMESPACE_BEGIN

class Component : private NonCopyable {
public:
private:
};

using ComponentId = u64;

#define DECLARE_COMPONENT(n)                                         \
    static Slice<u8 const> name() { return #n; }

GAME_NAMESPACE_END