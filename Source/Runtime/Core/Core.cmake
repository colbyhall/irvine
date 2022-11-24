# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/Core)

# Source files
set(CORE_SRC_FILES
	${CORE_ROOT}/Core.h
	${CORE_ROOT}/Core.cpp
	${CORE_ROOT}/NonCopyable.h
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    # Add natvis file
    set(CORE_SRC_FILES ${CORE_SRC_FILES} ${CORE_ROOT}/Core.natvis)
endif()

# Group source files
source_group(TREE ${CORE_ROOT} FILES ${CORE_SRC_FILES})

# Create Core lib
add_library(Core STATIC ${CORE_SRC_FILES})
target_include_directories(Core PUBLIC ${THIRD_PARTY_ROOT})
set_target_properties(Core PROPERTIES FOLDER "Runtime")
