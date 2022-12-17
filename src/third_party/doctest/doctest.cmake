# Set the root
set(DOCTEST_ROOT ${THIRD_PARTY_ROOT}/doctest)

# Source files
set(DOCTEST_SRC_FILES
	${DOCTEST_ROOT}/doctest.cmake
	${DOCTEST_ROOT}/doctest_internal.h
	${DOCTEST_ROOT}/doctest.h
	${DOCTEST_ROOT}/doctest.cpp
)

# Group source files
source_group(TREE ${DOCTEST_ROOT} FILES ${DOCTEST_SRC_FILES})

# Create Core lib
add_library(doctest STATIC ${DOCTEST_SRC_FILES})
target_include_directories(doctest PUBLIC ${RUNTIME_ROOT})
target_include_directories(doctest PUBLIC ${THIRD_PARTY_ROOT})
set_target_properties(doctest PROPERTIES FOLDER "third_party")
