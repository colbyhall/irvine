# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/context.h
	${GUI_ROOT}/context.cpp
	${GUI_ROOT}/gui.cmake
	${GUI_ROOT}/gui.h
	${GUI_ROOT}/window.h
	${GUI_ROOT}/window.cpp
	${GUI_ROOT}/widget/widget.h
	${GUI_ROOT}/widget/widget.cpp
)

# Group source files
source_group(TREE ${GUI_ROOT} FILES ${GUI_SRC_FILES})

# Create gui lib
add_library(gui STATIC ${GUI_SRC_FILES})
target_include_directories(gui PUBLIC ${RUNTIME_ROOT})
target_link_libraries(gui LINK_PUBLIC core)
target_link_libraries(gui LINK_PUBLIC gpu)
set_target_properties(gui PROPERTIES FOLDER "runtime")
