# Set the root
set(GUI_TESTS_ROOT ${TESTS_ROOT}/gui_tests)

# Source files
set(GUI_TESTS_SRC_FILES
	${GUI_TESTS_ROOT}/gui_tests.cmake
	${GUI_TESTS_ROOT}/gui_tests.cpp
)

# Group source files
source_group(TREE ${GUI_TESTS_ROOT} FILES ${GUI_TESTS_SRC_FILES})

add_executable(gui_tests ${GUI_TESTS_SRC_FILES})
target_include_directories(gui_tests PUBLIC ${RUNTIME_ROOT})
target_include_directories(gui_tests PUBLIC ${THIRD_PARTY_ROOT})
target_link_libraries(gui_tests LINK_PUBLIC core)
target_link_libraries(gui_tests LINK_PUBLIC gui)
target_link_libraries(gui_tests LINK_PUBLIC doctest)
set_target_properties(gui_tests PROPERTIES FOLDER "tests")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" AND NOT MINGW)
	target_link_options(gui_tests PUBLIC "/SUBSYSTEM:CONSOLE")
endif()

add_test(gui_tests gui_tests)
