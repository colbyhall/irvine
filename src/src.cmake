# Set the root
set(SRC_ROOT ${IRVINE_REPO_ROOT}/src)

# Set the programs root
set(PROGRAMS_ROOT ${SRC_ROOT}/programs)
# Set the runtime root
set(RUNTIME_ROOT ${SRC_ROOT}/runtime)
# Set the tests root
set(TESTS_ROOT ${SRC_ROOT}/tests)
# Set the third_party root
set(THIRD_PARTY_ROOT ${SRC_ROOT}/third_party)

# Enable folders for visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(${SRC_ROOT}/programs/programs.cmake)
include(${SRC_ROOT}/runtime/runtime.cmake)
include(${SRC_ROOT}/tests/tests.cmake)
include(${SRC_ROOT}/third_party/third_party.cmake)
