// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.h>
#include <gui/app.h>
#include <gui/builder.h>

#include <res/context.h>

#include <core/uuid.h>
#include <core/time.h>
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
	res::init();

	auto now = Instant::now();
	auto uuid = Uuid::generate();
	auto elapsed = now.elapsed().as_secs_f64() * 1000.0;
	(void)elapsed;

	auto app = gui::App::make();
	app.run([](gui::AppBuilder& ui) {
		gui::WindowConfig config = {
			.title = "Hello World"
		};
		ui.window(config, [](gui::Builder& ui) {
			ui.label("Hello World");
			ui.label("Hello World");
		});
	});
}
