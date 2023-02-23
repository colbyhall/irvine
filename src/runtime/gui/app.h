// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/function.h>

#include <gui/gui.h>
#include <gui/memory.h>
#include <gpu/graphics_pipeline.h>
#include <draw/font.h>

GUI_NAMESPACE_BEGIN

class App;
class Builder;
class AppBuilder;

class App {
public:
	static App make();
	void run(FunctionRef<void(AppBuilder&)> frame);

	FORCE_INLINE MemoryManager& memory() { return m_memory; }

private:
	App(gpu::GraphicsPipeline&& pipeline, draw::Font&& font) :
		m_pipeline(core::move(pipeline)),
		m_font(core::move(font))
		{}

	friend class Builder;
	friend class AppBuilder;

	MemoryManager m_memory;

	gpu::GraphicsPipeline m_pipeline;
	draw::Font m_font;
};

struct WindowConfig {
	StringView title;

	bool visible = true;
	bool maximized = false;
	bool minimized = false;
};

class AppBuilder {
public:
	AppBuilder(App& app) : m_app(app) {}

	void window(const WindowConfig& config, FunctionRef<void(Builder&)> contents);

private:
	App& m_app;
};

GUI_NAMESPACE_END
