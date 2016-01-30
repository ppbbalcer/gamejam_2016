#include "BossScreen.h"

BossScreen::BossScreen(RTexture *background)
{
	_background = background;
}

BossScreen::~BossScreen()
{

}

void BossScreen::OnUpdate(int delta)
{

}

void BossScreen::OnRender(SDL_Renderer* renderer, SDL_Rect screenRect)
{
	_background->setScaleWidth(screenRect.w);
	_background->setScaleHeight(screenRect.h);
	_background->renderAll(renderer, 0, 0);
}