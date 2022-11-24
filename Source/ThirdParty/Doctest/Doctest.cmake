# Set the root
set(DOCTEST_ROOT ${THIRD_PARTY_ROOT}/Doctest)

# Source files
set(DOCTEST_SRC_FILES
	${DOCTEST_ROOT}/Doctest.cmake
	${DOCTEST_ROOT}/doctest_internal.h
	${DOCTEST_ROOT}/Doctest.h
	${DOCTEST_ROOT}/Doctest.cpp
)

# Group source files
source_group(TREE ${DOCTEST_ROOT} FILES ${DOCTEST_SRC_FILES})

# Create Core lib
add_library(Doctest STATIC ${DOCTEST_SRC_FILES})
target_include_directories(Doctest PUBLIC ${RUNTIME_ROOT})
set_target_properties(Doctest PROPERTIES FOLDER "ThirdParty")
