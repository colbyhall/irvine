# Set the root
set(RES_ROOT ${RUNTIME_ROOT}/res)

# Source files
set(RES_SRC_FILES
	${RES_ROOT}/context.h
	${RES_ROOT}/context.cpp
	${RES_ROOT}/handle.h
	${RES_ROOT}/registry.h
	${RES_ROOT}/registry.cpp
	${RES_ROOT}/res.cmake
	${RES_ROOT}/res.h
	${RES_ROOT}/resource.h
)

# Group source files
source_group(TREE ${RES_ROOT} FILES ${RES_SRC_FILES})

# Create res lib
add_library(res STATIC ${RES_SRC_FILES})
target_include_directories(res PUBLIC ${RUNTIME_ROOT})
target_link_libraries(res LINK_PUBLIC core)
set_target_properties(res PROPERTIES FOLDER "runtime")
