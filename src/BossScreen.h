#ifndef __BOSS_SCREEN_H__
#define __BOSS_SCREEN_H__

#include "Engine/RTexture.h"
#include <SDL_render.h>
#include <string>

using namespace std;

class BossScreen
{
public:
	enum State
	{
		SHOWING,
		DANCING,
		HIDING,
		IDLE
	};

private:
	SDL_Rect	_screenRect;

	RTexture	*_background;
	RTexture	*_boss;

	State		_state;

	float		_timer;

	float		_bossPosX;
	float		_bossPosY;

	float		_bossDiePosY;
	float _boss_ratio;
	float _boss_target_ratio;
public:

	BossScreen(RTexture *background, RTexture *boss, SDL_Rect screenRect);
	~BossScreen();
	void OnUpdate(int delta);
	void OnRender(SDL_Renderer* renderer);
	void SetBossRatio(float x);
	void Kill();
};

#endif
