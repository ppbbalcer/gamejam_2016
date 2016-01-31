#include "BossScreen.h"

#define SHOWING_TIME	3000.0f
#define HIDING_TIME		500.0f
#define SHOWING_SPEED 0.0003f
BossScreen::BossScreen(RTexture *background, RTexture *boss, SDL_Rect screenRect)
:_screenRect(screenRect)
,_state(SHOWING)
,_timer(0)
,_bossPosX(0)
,_bossPosY(0)
,_background(background)
,_boss(boss)
,_boss_ratio(0)
,_boss_target_ratio(0)
{

}

BossScreen::~BossScreen()
{
	delete _background;
	delete _boss;
}
void BossScreen::SetBossRatio(float x)
{
	_boss_target_ratio = x;
}

void BossScreen::OnUpdate(int delta)
{
	_timer += delta;

	switch (_state)
	{
	case BossScreen::SHOWING:
		_boss_ratio += delta * SHOWING_SPEED;
		if (_boss_ratio >= _boss_target_ratio) {
			_boss_ratio = _boss_target_ratio;
		}
		if (_boss_ratio >= 1.0)
		{
			_boss_ratio = 1.0;
			_state = DANCING;
			_timer = 0;
		}
		
		_bossPosY = _screenRect.h - _screenRect.h * _boss_ratio + _screenRect.h * 0.2f;
		break;
	case BossScreen::DANCING:
		_bossPosY = (sin((_timer / 250.0f)/ M_PI) * _screenRect.h * 0.15f) + _screenRect.h * 0.2f;
		_bossPosX = (sin((_timer / 100.0f) / M_PI) * _screenRect.w * 0.2f);
		break;
	case BossScreen::HIDING:
		_boss_ratio = _timer / HIDING_TIME;
		if (_boss_ratio >= 1.0)
		{
			_boss_ratio = 1.0;
			_state = IDLE;
			_timer = 0;
		}
		_bossPosY = _bossDiePosY + _screenRect.h * _boss_ratio;
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
