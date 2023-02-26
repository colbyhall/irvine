// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.h>
#include <gui/app.h>
#include <gui/builder.h>
#include <core/platform/windows.h>

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
) {
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;

	gpu::init();
	auto app = gui::App::make();
	app.run([](gui::AppBuilder& ui) {
		gui::WindowConfig config = {
			.title = "Hello World"
		};
		ui.window(config, [](gui::Builder& ui) {
			ui.label("Hello World");
			ui.label("Hello World");
			ui.direction(gui::Direction::LeftToRight, [](auto& ui) {
				ui.heading("Hello World");
				ui.label("Hello World");
				ui.label("Hello World");
				ui.label("Hello World");
				ui.label("Hello World");
				ui.label("Hello World");
				ui.label("Hello World");
				ui.label("Hello World");
			});
		});
	});
}
