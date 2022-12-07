# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/core)

# Source files
set(CORE_SRC_FILES
	${CORE_ROOT}/core.cmake
	${CORE_ROOT}/core.hpp
	${CORE_ROOT}/core.cpp
	${CORE_ROOT}/memory.hpp
	${CORE_ROOT}/memory.cpp
	${CORE_ROOT}/non_copyable.hpp
	${CORE_ROOT}/async/atomic.hpp
	${CORE_ROOT}/containers/allocators.hpp
	${CORE_ROOT}/containers/array.hpp
	${CORE_ROOT}/containers/function.hpp
	${CORE_ROOT}/containers/non_null.hpp
	${CORE_ROOT}/containers/option.hpp
	${CORE_ROOT}/containers/result.hpp
	${CORE_ROOT}/containers/shared.hpp
	${CORE_ROOT}/containers/shared.inl
	${CORE_ROOT}/containers/slice.hpp
	${CORE_ROOT}/containers/string_view.hpp
	${CORE_ROOT}/containers/string_view.cpp
	${CORE_ROOT}/containers/string.hpp
	${CORE_ROOT}/containers/string.cpp
	${CORE_ROOT}/containers/unique.hpp
	${CORE_ROOT}/containers/wstring_view.hpp
	${CORE_ROOT}/containers/wstring.hpp
	${CORE_ROOT}/containers/wstring.cpp
	${CORE_ROOT}/math/aabb2.hpp
	${CORE_ROOT}/math/aabb2.inl
	${CORE_ROOT}/math/color.hpp
	${CORE_ROOT}/math/color.cpp
	${CORE_ROOT}/math/math.hpp
	${CORE_ROOT}/math/math.cpp
	${CORE_ROOT}/math/vec2.hpp
	${CORE_ROOT}/math/vec2.inl
	${CORE_ROOT}/math/vec3.hpp
	${CORE_ROOT}/math/vec3.inl
	${CORE_ROOT}/platform/library.hpp
	${CORE_ROOT}/platform/library.cpp
	${CORE_ROOT}/platform/windows.hpp
	${CORE_ROOT}/platform/time.hpp
	${CORE_ROOT}/platform/time.cpp
	${CORE_ROOT}/templates/decay.hpp
	${CORE_ROOT}/templates/enable_if.hpp
	${CORE_ROOT}/templates/invoke.hpp
	${CORE_ROOT}/templates/is_constructible.hpp
	${CORE_ROOT}/templates/is_invocable.hpp
	${CORE_ROOT}/templates/method_outer.hpp
	${CORE_ROOT}/templates/remove.hpp
	${CORE_ROOT}/templates/type_traits.hpp
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
