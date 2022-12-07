# Set the root
set(DRAW_ROOT ${RUNTIME_ROOT}/draw)

# Source files
set(DRAW_SRC_FILES
	${DRAW_ROOT}/canvas.hpp
	${DRAW_ROOT}/canvas.cpp
	${DRAW_ROOT}/draw.cmake
	${DRAW_ROOT}/draw.hpp
	${DRAW_ROOT}/font.hpp
	${DRAW_ROOT}/font.cpp
	${DRAW_ROOT}/shape.hpp
	${DRAW_ROOT}/shapes/rect.hpp
	${DRAW_ROOT}/shapes/rect.cpp
	${DRAW_ROOT}/shapes/text.hpp
	${DRAW_ROOT}/shapes/text.cpp
)

# Group source files
source_group(TREE ${DRAW_ROOT} FILES ${DRAW_SRC_FILES})

# Create draw lib
add_library(draw STATIC ${DRAW_SRC_FILES})
target_include_directories(draw PUBLIC ${RUNTIME_ROOT})
target_link_libraries(draw LINK_PUBLIC core gpu)
set_target_properties(draw PROPERTIES FOLDER "runtime")
