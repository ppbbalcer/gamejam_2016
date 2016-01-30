#ifndef __SCENE_GAME_H
#define __SCENE_GAME_H
#include <list>
#include "Engine/Scene.h"
#include "Engine/Engine.h"
#include "Engine/RTexture.h"
#include "Engine/PlayerInput.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "BossScreen.h"
#include <SDL.h>
#include "MapLogic/map.h"
class Fireball;
class Level;

// Uncomment following to enable structures supporting
//#define TWO_PLAYER_MODE

/**
 * scene that contains map and character
 */
class SceneGame: public Scene
{
private:
	SDL_Rect GetDefaultViewport();
	SDL_Rect GetGameplayViewport();
	SDL_Rect GetUIViewport();
	SDL_Rect GetBossViewport();

	std::list<Fireball *> fireballs;
//	RTexture *_background;
	Player *_player1;
#ifdef TWO_PLAYER_MODE
	Player *_player2;
#endif
	std::vector<Enemy*> _enemys;
	RTexture *_tiles;
	int _arrayShadowW;
	int _arrayShadowH;
	int *_arrayShadow;
	bool *_arrayVisibility;
	IMap *map;
	bool is_loaded;
	Level *level;
	int room_id;
	int heartbeat_tempo;
	SDL_Point _camera;
	BossScreen *_boss;

	void updateEnemies(int timems);
	void updateFireballs(int timems);
	void updatePlayers(int timems);
	void updateCamera();
	void updateShadowsChr(const Character *ch);
	void updateShadows();
	void renderGameplay(SDL_Renderer* renderer);
	void renderBoss(SDL_Renderer* renderer);
	void renderMap(SDL_Renderer* renderer);
	void renderShadow(SDL_Renderer* renderer);
	void renderGUI(SDL_Renderer *renderer);
	void drawBar(SDL_Renderer *renderer, int value, int playerBarYPadding, int playerBarHeight, int defaultX, int r, int g, int b) const;
	int dull(int) const;

public:
	SceneGame(Level *level, int room_id);
	virtual void OnLoad();
	virtual void OnFree();
	virtual void OnUpdate(int timems);
	virtual void OnRender(SDL_Renderer* renderer);
	virtual ~SceneGame();
};

bool IMap_isObstacle(int x, int y, void* objMap);

#endif
