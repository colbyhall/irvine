# Set the root
set(GAME_TESTS_ROOT ${TESTS_ROOT}/game_tests)

# Source files
set(GAME_TESTS_SRC_FILES
	${GAME_TESTS_ROOT}/game_tests.cmake
	${GAME_TESTS_ROOT}/game_tests.cpp
)

# Group source files
source_group(TREE ${GAME_TESTS_ROOT} FILES ${GAME_TESTS_SRC_FILES})

add_executable(game_tests ${GAME_TESTS_SRC_FILES})
target_include_directories(game_tests PUBLIC ${RUNTIME_ROOT})
target_include_directories(game_tests PUBLIC ${THIRD_PARTY_ROOT})
target_link_libraries(game_tests LINK_PUBLIC core gui doctest dxc draw game)
set_target_properties(game_tests PROPERTIES FOLDER "programs/tests")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" AND NOT MINGW)
	target_link_options(game_tests PUBLIC "/SUBSYSTEM:CONSOLE")
endif()

add_test(game_tests game_tests)
