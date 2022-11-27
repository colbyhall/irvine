# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES

)

# Group source files
source_group(TREE ${GUI_ROOT} FILES ${GUI_SRC_FILES})

# Create Core lib
add_library(gui STATIC ${GUI_SRC_FILES})
target_include_directories(gui PUBLIC ${RUNTIME_ROOT})
set_target_properties(gui PROPERTIES FOLDER "runtime")
