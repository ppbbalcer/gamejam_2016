#ifndef __BOSS_SCREEN_H__
#define __BOSS_SCREEN_H__

#include "Engine/RTexture.h"
#include <SDL_render.h>
#include <string>

using namespace std;

class BossScreen
{
private:
	RTexture	*_background;
public:

	BossScreen(RTexture *background);
	~BossScreen();
	void OnUpdate(int delta);
	void OnRender(SDL_Renderer* renderer, SDL_Rect screenRect);
};

#endif