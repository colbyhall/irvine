// Copyright Colby Hall. All Rights Reserved.

#include <gui/builder.h>
#include <gui/app.h>
#include <gui/draw.h>

GUI_NAMESPACE_BEGIN

void label(Builder& ui, const Label& label) {
	const f32 text_size = label.heading ? 24.f : 16.f;
	const auto size = ui.font().text_size(label.text, text_size, label.monospace);
	const auto bounds = ui.request_space(size);

	ui.canvas().text(
		bounds, 
		LinearColor::white, 
		TextShape {
			.text = String::from(label.text),
			.size = text_size,
		}
	);
}

MemoryManager& Builder::memory() {
	return m_app.m_memory;
}

const draw::Font& Builder::font() const {
	return m_app.m_font;
}

Aabb2f32 Builder::request_space(const Vec2f32& size) {
	switch (m_direction) {
	case Direction::TopToBottom: {
		const auto min = Vec2f32 { m_available.min().x, m_available.max().y - size.y };
		const auto max = Vec2f32 { m_available.min().x + size.x, m_available.max().y };

		m_available = Aabb2f32::from_min_max(
			m_available.min(),
			m_available.max() - Vec2f32 { 0.f, size.y }
		);

		return Aabb2f32::from_min_max(min, max);
	} break;
	case Direction::BottomToTop: {
		const auto min = m_available.min();
		const auto max = Vec2f32 { m_available.min().x + size.x, min.y + size.y };

		m_available = Aabb2f32::from_min_max(
			m_available.min() + Vec2f32{ 0.f, size.y },
			m_available.max()
		);

		return Aabb2f32::from_min_max(min, max);
	} break;
	case Direction::LeftToRight: {
		const auto min = Vec2f32 { m_available.min().x, m_available.max().y - size.y };
		const auto max = Vec2f32 { m_available.min().x + size.x, min.y + size.y };

		m_available = Aabb2f32::from_min_max(
			m_available.min() + Vec2f32{ size.x, 0.f },
			m_available.max()
		);

		return Aabb2f32::from_min_max(min, max);
	} break;
	case Direction::RightToLeft: {
		const auto min = Vec2f32 { m_available.max().x - size.x, m_available.max().y - size.y};
		const auto max = Vec2f32 { m_available.max().x, m_available.max().y };

		m_available = Aabb2f32::from_min_max(
			m_available.min(),
			m_available.max() - Vec2f32{ size.x, 0.f }
		);

		return Aabb2f32::from_min_max(min, max);
	} break;
	}

	return {};
}

void Builder::direction(Direction direction, FunctionRef<void(Builder&)> content) {
	auto builder = Builder(m_app, m_canvas, m_available);
	builder.m_direction = direction;
	content(builder);

	const auto diff = builder.m_bounds.size() - builder.m_available.size();
	request_space(diff);
}

GUI_NAMESPACE_END
