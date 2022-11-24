# Set the root
set(CORE_TESTS_ROOT ${TESTS_ROOT}/CoreTests)

# Source files
set(CORE_TESTS_SRC_FILES
	${CORE_TESTS_ROOT}/CoreTests.cmake
	${CORE_TESTS_ROOT}/CoreTests.cpp
	${CORE_TESTS_ROOT}/Containers/NonNull.cpp
)

# Group source files
source_group(TREE ${CORE_TESTS_ROOT} FILES ${CORE_TESTS_SRC_FILES})

add_executable(CoreTests ${CORE_TESTS_SRC_FILES})
target_include_directories(CoreTests PUBLIC ${RUNTIME_ROOT})
target_include_directories(CoreTests PUBLIC ${THIRD_PARTY_ROOT})
target_link_libraries(CoreTests LINK_PUBLIC Core)
target_link_libraries(CoreTests LINK_PUBLIC Doctest)
set_target_properties(CoreTests PROPERTIES FOLDER "Tests")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" AND NOT MINGW)
	target_link_options(CoreTests PUBLIC "/SUBSYSTEM:CONSOLE")
endif()

add_test(CoreTests CoreTests)
