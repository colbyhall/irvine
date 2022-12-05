// Copyright Colby Hall. All Rights Reserved.

#include <doctest/doctest.hpp>

TEST_MAIN()

#include <gui/window.hpp>

TEST_CASE("guis can create windows") {
	gui::WindowConfig config = {
		.title = "Test",
	};
	auto window = gui::make_window(config);

	for (;;) {
		gui::pump_events();
	}
}
