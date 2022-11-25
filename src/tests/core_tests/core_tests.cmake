# Set the root
set(CORE_TESTS_ROOT ${TESTS_ROOT}/core_tests)

# Source files
set(CORE_TESTS_SRC_FILES
	${CORE_TESTS_ROOT}/core_tests.cmake
	${CORE_TESTS_ROOT}/core_tests.cpp
	${CORE_TESTS_ROOT}/containers/non_null.cpp
	${CORE_TESTS_ROOT}/containers/slice.cpp
)

# Group source files
source_group(TREE ${CORE_TESTS_ROOT} FILES ${CORE_TESTS_SRC_FILES})

add_executable(core_tests ${CORE_TESTS_SRC_FILES})
target_include_directories(core_tests PUBLIC ${RUNTIME_ROOT})
target_include_directories(core_tests PUBLIC ${THIRD_PARTY_ROOT})
target_link_libraries(core_tests LINK_PUBLIC core)
target_link_libraries(core_tests LINK_PUBLIC coctest)
set_target_properties(core_tests PROPERTIES FOLDER "tests")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" AND NOT MINGW)
	target_link_options(core_tests PUBLIC "/SUBSYSTEM:CONSOLE")
endif()

add_test(core_tests core_tests)
