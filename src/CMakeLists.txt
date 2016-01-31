cmake_minimum_required(VERSION 2.8)
project(base_game_bin)

include_directories(
	${base_game_bin_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

# why do you incluide SDL parts in repository?
include_directories(
	"/usr/include/SDL2/"
	${SDL2_INCLUDE_DIR}
)

set(client_src
	BossScreen.cpp
	BossScreen.h
	Character.cpp
	Character.h
	Enemy.cpp
	Enemy.h
	fireball.cpp
	fireball.h
	GlobalData.cpp
	GlobalData.h
	level.cpp
	level.h
	main.cpp
	Player.cpp
	Player.h
	SceneExamples.h
	SceneFont.h
	scene_game.cpp
	scene_game.h
	victoryscene.cpp
	victoryscene.h
		Equipment.cpp Equipment.h Item.cpp Item.h
	menuscene.cpp
	menuscene.h)

add_subdirectory(Engine)
add_subdirectory(MapLogic)

add_executable(client ${client_src})

find_package(OpenGL REQUIRED)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMG REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2MIX REQUIRED SDL2_mixer>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

target_link_libraries(client ${SDL2_LIBRARIES} ${SDL2IMG_LIBRARIES} ${SDL2MIX_LIBRARIES} ${SDL2TTF_LIBRARIES} ${OPENGL_LIBRARIES} engine MapLogic m)

