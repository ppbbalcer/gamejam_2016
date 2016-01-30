#ifndef __MENU_SCENE_H
#define __MENU_SCENE_H

#include "Engine/Scene.h"
#include "level.h"

enum menu_option {
	MENU_PLAY,
	MENU_MAP_SELECT,
	MENU_OPTIONS,
	MENU_EXIT,

	MAX_MENU_OPTION
};

enum menu_mode {
	MENU_MODE_NORMAL,
	MENU_MODE_SELECT,

	MAX_MENU_MODE
};

class MenuScene: public Scene {
public:
	MenuScene(int world, int lvl);
	virtual ~MenuScene();

	virtual void OnLoad() {

	}

	virtual void OnFree(){
	}

	virtual void OnUpdate(int timems);
	virtual void OnRender(SDL_Renderer* renderer);
private:
	Level *level;
	int world;
	int lvl;

	void handleNormalModeInput();
	void handleSelectModeInput();

	enum menu_mode mode;
	enum menu_option current;
};


#endif
