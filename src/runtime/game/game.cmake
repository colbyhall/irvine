# Set the root
set(GAME_ROOT ${RUNTIME_ROOT}/game)

# Source files
set(GAME_SRC_FILES
	${GAME_ROOT}/game.cmake
	${GAME_ROOT}/game.hpp
	${GAME_ROOT}/ecs/archetype.hpp
	${GAME_ROOT}/ecs/component.hpp
	${GAME_ROOT}/ecs/entity.hpp
	${GAME_ROOT}/ecs/world.hpp
)

# Group source files
source_group(TREE ${GAME_ROOT} FILES ${GAME_SRC_FILES})

# Create game lib
add_library(game STATIC ${GAME_SRC_FILES})
target_include_directories(game PUBLIC ${RUNTIME_ROOT})
target_link_libraries(game LINK_PUBLIC core gpu gui)
set_target_properties(game PROPERTIES FOLDER "runtime")
