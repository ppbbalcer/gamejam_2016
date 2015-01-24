#include "level.h"

Level::Level(int id) {
	this->id = id;
	current_scene = -1;
}

Level::~Level() {
	for (scenes_map_t::iterator itr = scenes.begin(); itr != scenes.end(); ++itr) {
		delete itr->second;
	}
	scenes.clear();
}

SceneGame* Level::getCurrentScene() {
	if (current_scene == -1) //first run
		setCurrentScene(INITIAL_LEVEL_SCENE);
	return scenes[current_scene];
}

void Level::setCurrentScene(int scene_id) {
	if (scenes.find(scene_id) == scenes.end()) {
		scenes[scene_id] = new SceneGame(this, scene_id);
	}
	current_scene = scene_id;
	EngineInst->setNextScene(getCurrentScene());
}

void Level::resetCurrent() {
	delete scenes[current_scene];
	scenes.erase(current_scene);
	setCurrentScene(current_scene);
}