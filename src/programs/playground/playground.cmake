# Set the root
set(PLAYGROUND_ROOT ${PROGRAMS_ROOT}/playground)

# Source files
set(PLAYGROUND_SRC_FILES
	${PLAYGROUND_ROOT}/playground.cmake
	${PLAYGROUND_ROOT}/playground.cpp
)

# Group source files
source_group(TREE ${PLAYGROUND_ROOT} FILES ${PLAYGROUND_SRC_FILES})

add_executable(playground ${PLAYGROUND_SRC_FILES})
target_include_directories(playground PUBLIC ${RUNTIME_ROOT})
target_include_directories(playground PUBLIC ${THIRD_PARTY_ROOT})
target_link_libraries(playground LINK_PUBLIC core gui gpu dxc)
set_target_properties(playground PROPERTIES FOLDER "programs")
