# Set the root
set(STB_ROOT ${THIRD_PARTY_ROOT}/stb)

# Source files
set(STB_SRC_FILES
	${STB_ROOT}/stb.cmake
	${STB_ROOT}/stb_image_write.h
	${STB_ROOT}/stb_image.h
	${STB_ROOT}/stb_implementation.cpp
	${STB_ROOT}/stb_rect_pack.h
	${STB_ROOT}/stb_truetype.h
)

# Group source files
source_group(TREE ${STB_ROOT} FILES ${STB_SRC_FILES})

# Create stb lib
add_library(stb STATIC ${STB_SRC_FILES})
target_include_directories(stb PUBLIC ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
target_link_libraries(stb LINK_PUBLIC core)
set_target_properties(stb PROPERTIES FOLDER "third_party")
