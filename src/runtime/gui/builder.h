// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gui/gui.h>
#include <draw/draw.h>
#include <core/math/aabb2.h>
#include <core/containers/string.h>

// Draw forward declaration
DRAW_NAMESPACE_BEGIN
class Font;
DRAW_NAMESPACE_END

GUI_NAMESPACE_BEGIN

// Gui forward declarations
class App;
class MemoryManager;
class Canvas;
class Builder;

struct Label {
	StringView text;
	bool heading = false;
	bool monospace = false;
};
void label(Builder& ui, const Label& label);

class Builder {
public:
	Builder(App& app, Canvas& canvas, const Aabb2f32& bounds) 
		: m_app(app), m_canvas(canvas), m_bounds(bounds) {};

	MemoryManager& memory();
	const draw::Font& font() const;
	FORCE_INLINE Canvas& canvas() { return m_canvas; }
	Aabb2f32 request_space(const Vec2f32& size);

	FORCE_INLINE void label(StringView text) {
		gui::label(*this, Label { .text = text });
	}
	FORCE_INLINE void heading(StringView text) {
		gui::label(*this, Label { .text = text, .heading = true });
	}

private:
	App& m_app;
	Canvas& m_canvas;
	Aabb2f32 m_bounds;
};

GUI_NAMESPACE_END
