include_directories(
	${base_game_bin_SOURCE_DIR}
)

include_directories(
	${SDL2_INCLUDE_DIR}
)

set(engine_src
	AStar.cpp
	AStar.h
	Audio.cpp
	Audio.h
	Engine.cpp
	Engine.h
	RFont.cpp
	RFont.h
	RTexture.cpp
	RTexture.h
	Scene.h
	StructsGlobal.h
	PlayerInput.cpp
	PlayerInput.h
)

add_library(engine ${engine_src})
