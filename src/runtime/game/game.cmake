# Set the root
set(GAME_ROOT ${RUNTIME_ROOT}/game)

# Source files
set(GAME_SRC_FILES
	${GAME_ROOT}/game.cmake
	${GAME_ROOT}/game.h
	${GAME_ROOT}/transform.h
	${GAME_ROOT}/ecs/archetype.h
	${GAME_ROOT}/ecs/archetype.cpp
	${GAME_ROOT}/ecs/archetype_id.h
	${GAME_ROOT}/ecs/archetype_id.cpp
	${GAME_ROOT}/ecs/component.h
	${GAME_ROOT}/ecs/component.cpp
	${GAME_ROOT}/ecs/entity.h
	${GAME_ROOT}/ecs/storage.h
	${GAME_ROOT}/ecs/world.h
	${GAME_ROOT}/ecs/world.cpp
)

# Group source files
source_group(TREE ${GAME_ROOT} FILES ${GAME_SRC_FILES})

# Create game lib
add_library(game STATIC ${GAME_SRC_FILES})
target_include_directories(game PUBLIC ${RUNTIME_ROOT})
target_link_libraries(game LINK_PUBLIC core gpu gui)
set_target_properties(game PROPERTIES FOLDER "runtime")
