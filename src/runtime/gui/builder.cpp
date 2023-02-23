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
	const auto max = Vec2f32 { m_bounds.min().x + size.x, m_bounds.max().y };
	const auto min = Vec2f32 { m_bounds.min().x, m_bounds.max().y - size.y };

	m_bounds = Aabb2f32::from_min_max(
		m_bounds.min(), 
		m_bounds.max() - Vec2f32 { 0.f, size.y }
	);

	return Aabb2f32::from_min_max(min, max);
}

GUI_NAMESPACE_END
