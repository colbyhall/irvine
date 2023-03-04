// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/math/vec2.h>

CORE_NAMESPACE_BEGIN

template <typename T>
class Aabb2 {
public:
	constexpr Aabb2() = default;

	constexpr static Aabb2<T> from_center(const Vec2<T>& location, const Vec2<T>& half_size);
	constexpr static Aabb2<T> from_min_max(const Vec2<T>& min, const Vec2<T>& max);

	inline Vec2<T> bottom_left() const { return m_position - m_half_size; }
	inline Vec2<T> top_right() const { return m_position + m_half_size; }
	inline Vec2<T> bottom_right() const;
	inline Vec2<T> top_left() const;

	inline Vec2<T> min() const { return bottom_left(); }
	inline Vec2<T> max() const { return top_right(); }

	inline Vec2<T> position() const { return m_position; }
	inline Vec2<T> half_size() const { return m_half_size; }
	inline Vec2<T> size() const { return m_half_size * (T)2; }

private:
	inline constexpr Aabb2(const Vec2<T>& position, const Vec2<T>& half_size)
		: m_position(position), m_half_size(half_size) { }

	Vec2<T> m_position;
	Vec2<T> m_half_size;
};

CORE_NAMESPACE_END

#include <core/math/aabb2.inl>

using Aabb2f32 = core::Aabb2<f32>;
using Aabb2f64 = core::Aabb2<f64>;
using Aabb2u32 = core::Aabb2<u32>;
using Aabb2i32 = core::Aabb2<i32>;
