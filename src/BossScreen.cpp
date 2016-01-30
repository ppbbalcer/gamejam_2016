#include "BossScreen.h"

#define SHOWING_TIME	3000.0f
#define HIDING_TIME		500.0f

BossScreen::BossScreen(RTexture *background, RTexture *boss, SDL_Rect screenRect)
:_screenRect(screenRect)
,_state(SHOWING)
,_timer(0)
,_bossPosX(0)
,_bossPosY(0)
,_background(background)
,_boss(boss)
{

}

BossScreen::~BossScreen()
{
	delete _background;
	delete _boss;
}

void BossScreen::OnUpdate(int delta)
{
	_timer += delta;
	float ratio = 0.0f;

	switch (_state)
	{
	case BossScreen::SHOWING:
		ratio = _timer / SHOWING_TIME;
		if (ratio >= 1.0)
		{
			ratio = 1.0;
			_state = DANCING;
			_timer = 0;
		}
		_bossPosY = _screenRect.h - _screenRect.h * ratio + _screenRect.h * 0.2f;
		break;
	case BossScreen::DANCING:
		_bossPosY = (sin((_timer / 250.0f)/ M_PI) * _screenRect.h * 0.15f) + _screenRect.h * 0.2f;
		_bossPosX = (sin((_timer / 100.0f) / M_PI) * _screenRect.w * 0.2f);
		break;
	case BossScreen::HIDING:
		ratio = _timer / HIDING_TIME;
		if (ratio >= 1.0)
		{
			ratio = 1.0;
			_state = IDLE;
			_timer = 0;
		}
		_bossPosY = _bossDiePosY + _screenRect.h * ratio;
		break;
	default:
		break;
	}
}

void BossScreen::Kill()
{
	_state = HIDING;
	_timer = 0.0f;
	_bossDiePosY = _bossPosY;
}

void BossScreen::OnRender(SDL_Renderer* renderer)
{
	_background->setScaleWidth(_screenRect.w);
	_background->setScaleHeight(_screenRect.h);
	_boss->setScaleSize(_screenRect.w / _boss->getWidth());
	_background->renderAll(renderer, 0, 0);
	_boss->renderAll(renderer, _bossPosX, _bossPosY);
}