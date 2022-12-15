# Set the tests root
set(TESTS_ROOT ${PROGRAMS_ROOT}/tests)

enable_testing()

include(${TESTS_ROOT}/core_tests/core_tests.cmake)
include(${TESTS_ROOT}/game_tests/game_tests.cmake)
include(${TESTS_ROOT}/gui_tests/gui_tests.cmake)

