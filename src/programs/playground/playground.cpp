// Copyright Colby Hall. All Rights Reserved.

#include <gui/window.hpp>

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	gui::WindowConfig config = {
		.title = "Test",
	};
	auto window = gui::Window::make(config);

	while(true) {
		gui::pump_events();
	}

	return 0;
}
