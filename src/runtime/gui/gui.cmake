# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES
	${GUI_ROOT}/app.h
	${GUI_ROOT}/app.cpp
	${GUI_ROOT}/builder.h
	${GUI_ROOT}/builder.cpp
	${GUI_ROOT}/draw.h
	${GUI_ROOT}/draw.cpp
	${GUI_ROOT}/gui.cmake
	${GUI_ROOT}/gui.h
	${GUI_ROOT}/id.h
	${GUI_ROOT}/id.cpp
	${GUI_ROOT}/memory.h
	${GUI_ROOT}/memory.cpp
)

# Group source files
source_group(TREE ${GUI_ROOT} FILES ${GUI_SRC_FILES})

# Create gui lib
add_library(gui STATIC ${GUI_SRC_FILES})
target_include_directories(gui PUBLIC ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
target_link_libraries(gui LINK_PUBLIC core gpu dxc draw)
set_target_properties(gui PROPERTIES FOLDER "runtime")
