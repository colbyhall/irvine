# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/Core)

# Source files
set(CORE_SRC_FILES
	${CORE_ROOT}/Core.cmake
	${CORE_ROOT}/Core.h
	${CORE_ROOT}/Core.cpp
	${CORE_ROOT}/NonCopyable.h
	${CORE_ROOT}/Containers/NonNull.h
	${CORE_ROOT}/Containers/NonNull.inl
	${CORE_ROOT}/Containers/Slice.h
	${CORE_ROOT}/Containers/Slice.inl
	${CORE_ROOT}/Platform/Windows.h
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    # Add natvis file
    set(CORE_SRC_FILES ${CORE_SRC_FILES} ${CORE_ROOT}/Core.natvis)
endif()

# Group source files
source_group(TREE ${CORE_ROOT} FILES ${CORE_SRC_FILES})

# Create Core lib
add_library(Core STATIC ${CORE_SRC_FILES})
target_include_directories(Core PUBLIC ${RUNTIME_ROOT})
set_target_properties(Core PROPERTIES FOLDER "Runtime")
