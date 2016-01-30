#include <list>
#include "Engine/Scene.h"
#include "Engine/Engine.h"
#include "Engine/RTexture.h"
#include <SDL.h>
#include "GlobalData.h"
#include "menuscene.h"

MenuScene::MenuScene(int _world, int _lvl) : world(_world), lvl(_lvl) {
	current = MENU_PLAY;
	level = NULL;
	mode = MENU_MODE_NORMAL;
}

MenuScene::~MenuScene() {
	if (level)
		delete level;
}

void MenuScene::handleNormalModeInput() {
	if (EngineInst->input()->getState(INPUT_MENU_ENTER)) {
		switch (current) {
		case MENU_PLAY:
			level = new Level(world, lvl);
			EngineInst->setNextScene(level->getCurrentScene());
			break;
		case MENU_MAP_SELECT:
			mode = MENU_MODE_SELECT;
			break;
		case MENU_OPTIONS:
			break;
		case MENU_EXIT:
			EngineInst->breakMainLoop();
			return;
		}
	}

	if (EngineInst->input()->getState(INPUT_MENU_DOWN)) {
		current = (menu_option)(current + 1);
		if (current == MAX_MENU_OPTION)
			current = (menu_option)(MAX_MENU_OPTION - 1);
	}

	if (EngineInst->input()->getState(INPUT_MENU_UP)) {
		if (current != 0)
			current = (menu_option)(current - 1);
	}
}

#define WORLDS 3
int worlds_levels[WORLDS] = {
	5, 14, 9
};

void MenuScene::handleSelectModeInput() {
	if (EngineInst->input()->getState(INPUT_MENU_ENTER)) {
		level = new Level(world, lvl);
		EngineInst->setNextScene(level->getCurrentScene());

		return;
	}

	if (EngineInst->input()->getState(INPUT_MENU_BACK)) {
		mode = MENU_MODE_NORMAL;
	}

	if (EngineInst->input()->getState(INPUT_MENU_DOWN)) {
		lvl++;
		if (lvl == worlds_levels[world])
			lvl = worlds_levels[world] - 1;
	}

	if (EngineInst->input()->getState(INPUT_MENU_UP)) {
		lvl--;
		if (lvl < 0)
			lvl = 0;
	}
	if (EngineInst->input()->getState(INPUT_MENU_LEFT)) {
		world--;
		if (world == 0)
			world = 0;
	}

	if (EngineInst->input()->getState(INPUT_MENU_RIGHT)) {
		world++;
		if (world == WORLDS)
			world = WORLDS - 1;
	}
}

void MenuScene::OnUpdate(int timems) {
	if (mode == MENU_MODE_NORMAL)
		handleNormalModeInput();
	else if (mode == MENU_MODE_SELECT)
		handleSelectModeInput();
};

SDL_Color color_black = { 0, 0, 0, 255 };
SDL_Color color_grey = {204, 204, 204, 255 };
SDL_Color color_red = {255, 50, 50, 255 };
SDL_Color color_blue = {50, 50, 255, 255};

SDL_Color color_disabled = { 90, 90, 90, 255 };
SDL_Color color_disabled_red = { 120, 0, 0, 255 };

struct menu_item {
	char *text;
} menu[MAX_MENU_OPTION] = {
	"Play",
	"Map select",
	"Options",
	"Exit"
};


void MenuScene::OnRender(SDL_Renderer* renderer) {
	EngineInst->font()->printf(0, 0, color_blue, color_black, 0, ALIGN_TOP | ALIGN_LEFT, 3.f, "Break the ritual");

	for (int i = 0; i < MAX_MENU_OPTION; ++i) {
		EngineInst->font()->printf(30, 200 + ((i) * 20), i == current ? color_red : color_grey, color_black, 0, ALIGN_TOP | ALIGN_LEFT, 1.f, menu[i].text);
	}

	EngineInst->font()->printf(200, 220, mode == MENU_MODE_SELECT ? color_red : color_disabled, color_black, 0, ALIGN_TOP | ALIGN_LEFT, 1.f, "World: ");

	for (int i = 0; i < WORLDS; ++i) {
		char buf[255];
		sprintf(buf, "%d", i);
		EngineInst->font()->printf(250 + ((i+1) * 30), 220, i == world ? color_grey : color_disabled, color_black, 0, ALIGN_TOP | ALIGN_LEFT, 1.f, buf);
		if (i == world) {
			for (int j = 0; j < worlds_levels[i]; ++j) {
				sprintf(buf, "%d", j);
				EngineInst->font()->printf(250 + ((i+1)* 30), 220 + ((j+1)*20), j == lvl ? color_disabled_red : color_disabled, color_black, 0, ALIGN_TOP | ALIGN_LEFT, 1.f, buf);
			}
		}
	}

};
