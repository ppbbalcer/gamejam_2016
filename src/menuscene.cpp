#include <list>
#include "Engine/Scene.h"
#include "Engine/Engine.h"
#include "Engine/RTexture.h"
#include <SDL.h>
#include "GlobalData.h"
#include "menuscene.h"

MenuScene::MenuScene(int world, int lvl) {
	level = new Level(world, lvl);
	current = MENU_PLAY;
}

MenuScene::~MenuScene() {
	delete level;
}

void MenuScene::OnUpdate(int timems) {
	if (EngineInst->input()->getState(INPUT_MENU_ENTER)) {
		switch (current) {
		case MENU_PLAY:
			EngineInst->setNextScene(level->getCurrentScene());
			break;
		case MENU_MAP_SELECT:
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
};

SDL_Color color_black = { 255, 255, 255, 255 };
SDL_Color color_grey = {204, 204, 204, 255 };
SDL_Color color_red = {255, 50, 50, 255 };
SDL_Color color_blue = {50, 50, 255, 255};

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
};
