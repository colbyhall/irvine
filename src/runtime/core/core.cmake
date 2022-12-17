# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/core)

# Source files
set(CORE_SRC_FILES
	${CORE_ROOT}/core.cmake
	${CORE_ROOT}/core.h
	${CORE_ROOT}/core.cpp
	${CORE_ROOT}/hash.h
	${CORE_ROOT}/hash.cpp
	${CORE_ROOT}/memory.h
	${CORE_ROOT}/memory.cpp
	${CORE_ROOT}/non_copyable.h
	${CORE_ROOT}/source_location.h
	${CORE_ROOT}/containers/allocators.h
	${CORE_ROOT}/containers/array.h
	${CORE_ROOT}/containers/array.inl
	${CORE_ROOT}/containers/bucket_array.h
	${CORE_ROOT}/containers/bucket_array.inl
	${CORE_ROOT}/containers/function.h
	${CORE_ROOT}/containers/hash_map.h
	${CORE_ROOT}/containers/hash_map.inl
	${CORE_ROOT}/containers/non_null.h
	${CORE_ROOT}/containers/option.h
	${CORE_ROOT}/containers/result.h
	${CORE_ROOT}/containers/shared.h
	${CORE_ROOT}/containers/shared.inl
	${CORE_ROOT}/containers/slice.h
	${CORE_ROOT}/containers/slot_map.h
	${CORE_ROOT}/containers/slot_map.inl
	${CORE_ROOT}/containers/string_view.h
	${CORE_ROOT}/containers/string_view.cpp
	${CORE_ROOT}/containers/string.h
	${CORE_ROOT}/containers/string.cpp
	${CORE_ROOT}/containers/unique.h
	${CORE_ROOT}/containers/wstring_view.h
	${CORE_ROOT}/containers/wstring.h
	${CORE_ROOT}/containers/wstring.cpp
	${CORE_ROOT}/fs/dir.hpp
	${CORE_ROOT}/fs/dir.cpp
	${CORE_ROOT}/fs/file.h
	${CORE_ROOT}/fs/file.cpp
	${CORE_ROOT}/fs/path_view.h
	${CORE_ROOT}/fs/path.h
	${CORE_ROOT}/math/aabb2.h
	${CORE_ROOT}/math/aabb2.inl
	${CORE_ROOT}/math/color.h
	${CORE_ROOT}/math/color.cpp
	${CORE_ROOT}/math/mat3.h
	${CORE_ROOT}/math/mat3.inl
	${CORE_ROOT}/math/mat4.h
	${CORE_ROOT}/math/mat4.inl
	${CORE_ROOT}/math/math.h
	${CORE_ROOT}/math/math.cpp
	${CORE_ROOT}/math/quat.h
	${CORE_ROOT}/math/quat.inl
	${CORE_ROOT}/math/vec2.h
	${CORE_ROOT}/math/vec2.inl
	${CORE_ROOT}/math/vec3.h
	${CORE_ROOT}/math/vec3.inl
	${CORE_ROOT}/math/vec4.h
	${CORE_ROOT}/math/vec4.inl
	${CORE_ROOT}/platform/library.h
	${CORE_ROOT}/platform/library.cpp
	${CORE_ROOT}/platform/windows.h
	${CORE_ROOT}/platform/time.h
	${CORE_ROOT}/platform/time.cpp
	${CORE_ROOT}/sync/atomic.h
	${CORE_ROOT}/templates/decay.h
	${CORE_ROOT}/templates/enable_if.h
	${CORE_ROOT}/templates/invoke.h
	${CORE_ROOT}/templates/is_constructible.h
	${CORE_ROOT}/templates/is_invocable.h
	${CORE_ROOT}/templates/method_outer.h
	${CORE_ROOT}/templates/remove.h
	${CORE_ROOT}/templates/type_traits.h
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    # Add natvis file
    set(CORE_SRC_FILES ${CORE_SRC_FILES} ${CORE_ROOT}/core.natvis)
endif()

# Group source files
source_group(TREE ${CORE_ROOT} FILES ${CORE_SRC_FILES})

# Create Core lib
add_library(core STATIC ${CORE_SRC_FILES})
target_include_directories(core PUBLIC ${RUNTIME_ROOT})
set_target_properties(core PROPERTIES FOLDER "runtime")
