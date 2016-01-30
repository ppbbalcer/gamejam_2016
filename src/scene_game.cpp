#include "MapLogic/field.h"
#include "MapLogic/map.h"
#include "scene_game.h"
#include "GlobalData.h"
#include <algorithm>
#include "Engine/AStar.h"
#include <stdio.h>
#include "fireball.h"
#include "Enemy.h"
#include "level.h"
#include "MapLogic/door.h"
#include "MapLogic/stairs.h"
using namespace std;

#define MAX_ROOM_PATH 255
#define HEARTBEAT_BASE_INTERVAL 2000
#define HEARTBEAT_MIN_INTERVAL 500
//#define DEBUG_BOTS
#define MAX_TILE_PER_SCREEN 12

#define UI_HEIGHT	0.15f
#define GP_HEIGHT	1.0f - UI_HEIGHT
#define GP_START_Y	UI_HEIGHT
#define GP_WIDTH_RATIO	1.3f

// Global
IMap *gCurrentMap = NULL;


SceneGame::SceneGame(Level *level, int room_id)
{
	this->room_id = room_id;
	this->level = level;
	this->heartbeat_tempo = 0;
	this->_camera.x = this->_camera.y = 0;
	char buff[MAX_ROOM_PATH];
	sprintf(buff, "Resources/levels/%u/%u.txt", level->getId(), room_id);
	map = IMap::Factory(IMap::LOADED, buff);

	gCurrentMap = map;

	is_loaded = false;
}

SceneGame::~SceneGame()
{
	delete map;
}
SDL_Rect SceneGame::GetDefaultViewport()
{
#define MARGIN_TOP 5
#define MARGIN_BOTTOM 5
#define MARGIN_LEFT 5
#define MARGIN_RIGHT 5

	SDL_Rect topLeftViewport;
	topLeftViewport.x = MARGIN_LEFT;
	topLeftViewport.y = MARGIN_TOP;
	topLeftViewport.w = EngineInst->screen_width()-MARGIN_LEFT-MARGIN_RIGHT;
	topLeftViewport.h = EngineInst->screen_height()-MARGIN_TOP-MARGIN_BOTTOM;
	return topLeftViewport;
}

SDL_Rect SceneGame::GetGameplayViewport()
{
	SDL_Rect topLeftViewport = GetDefaultViewport();
	topLeftViewport.y = GP_START_Y * topLeftViewport.h;
	topLeftViewport.h *= GP_HEIGHT;
	topLeftViewport.x = topLeftViewport.w - topLeftViewport.h * GP_WIDTH_RATIO;
	topLeftViewport.w = topLeftViewport.h * GP_WIDTH_RATIO;
	return topLeftViewport;
}

SDL_Rect SceneGame::GetUIViewport()
{
	SDL_Rect veryTopBar;
	veryTopBar.x = 0;
	veryTopBar.y = 0;
	veryTopBar.w = EngineInst->screen_width();
	veryTopBar.h = EngineInst->screen_height() * UI_HEIGHT;
	return veryTopBar;
}

SDL_Rect SceneGame::GetBossViewport()
{
	SDL_Rect topLeftViewport = GetDefaultViewport();
	topLeftViewport.y = GP_START_Y * topLeftViewport.h;
	topLeftViewport.h *= GP_HEIGHT;
	topLeftViewport.x = 0;
	topLeftViewport.w = topLeftViewport.w - topLeftViewport.h * GP_WIDTH_RATIO;
	return topLeftViewport;
}

void SceneGame::OnLoad()
{
	/* command generating set of tiles found in Resources/tiles/walls.png
	 * (to be invoked from Resources/tiles/walls directory)
	 *  montage *.png -background none ../floor0.png -geometry +0x0 -tile 3x3 ../walls.png
	 */
	EngineInst->clearStatusLine();
	SDL_Rect dvp=GetDefaultViewport();
	/* scale entire board
	 */
	int tile_size = std::min<int>(dvp.w / MAX_TILE_PER_SCREEN,
				 dvp.h / MAX_TILE_PER_SCREEN);

	EngineInst->setTileSize(tile_size);
	bool success = EngineInst->loadResources(texturesScene_game, texturesScene_gameSize);
	/* slightly bigger than usually displayed, for small maps on large resolutions*/
	int tileSizeSrc = 64;

	EngineInst->setStatusLine("mission hint");

	RTexture *tmpTexture;
	/*_background = new RTexture(texturesScene_game[1]);
	_background ->setScaleSize(1.0f * EngineInst->screen_width() / _background->getWidth());*/

	/*Texture section for player 1*/
	tmpTexture = new RTexture(texturesScene_game[3]);
	tmpTexture->setTileSizeSrc(tileSizeSrc);
	tmpTexture->setTileSizeDst(tile_size);
	tmpTexture->setTileIdx(24);
	_player1 = new Player(tmpTexture, map, map->getParams()->start_hp, map->getParams()->start_mana);

	tmpTexture = new RTexture(texturesScene_game[3]);
	tmpTexture->setTileSizeSrc(tileSizeSrc);
	tmpTexture->setTileSizeDst(tile_size);
	tmpTexture->setTileIdx(27);
	#ifdef TWO_PLAYER_MODE
	_player2 = new Player(tmpTexture, map, map->getParams()->start_hp, map->getParams()->start_mana);
	#endif
	/* starting location is loaded from map configuration file */
	_player1->setPosTiles(map->GetPlayer1Start().first,
			      map->GetPlayer1Start().second);
	#ifdef TWO_PLAYER_MODE
	_player2->setPosTiles(map->GetPlayer2Start().first,
			      map->GetPlayer2Start().second);
	#endif /* TWO_PLAYER_MODE */

	EngineInst->setStatusLine(map->GetTitleString());

	/* upon first load of this particular map place all enemies */
	if (!is_loaded) {
		const enemies_list &ens = map->GetEnemies();

		for (enemies_list::const_iterator it=ens.begin()
			     ;it!=ens.end();
		     ++it)
		{
			tmpTexture = new RTexture(texturesScene_game[9]);
			tmpTexture->setTileSizeSrc(tileSizeSrc);
			tmpTexture->setTileSizeDst(tile_size);
			tmpTexture->setTileIdx(1);
			Enemy* enemy = new Enemy(tmpTexture, map, (*it)->hp, (*it)->ai);
			enemy->setPosTiles((*it)->x, (*it)->y);
			_enemys.push_back(enemy);
		}
	}

	/* tiles for map */
	_tiles = new RTexture(texturesScene_game[3]);
	_tiles->setTileSizeSrc(tileSizeSrc);
	_tiles->setTileSizeDst(tile_size);
	/* lighting setup */
	_arrayShadowW = map->GetWidth();
	_arrayShadowH = map->GetHeight();
	_arrayShadow = new int[_arrayShadowW*_arrayShadowH*sizeof(_arrayShadowH)];
	_arrayVisibility = new bool[_arrayShadowW*_arrayShadowH*sizeof(bool)];
	memset(_arrayVisibility, 0, _arrayShadowW*_arrayShadowH*sizeof(bool));
	//Load media
	if (!success) {
		printf("Failed to load media Scene02Renderer !\n");
		PAUSE();
	}
	globalAudios[HEARTBEAT].res.sound->setVolume(0.2f);
	globalAudios[HEARTBEAT].res.sound->play(-1, 0, HEARTBEAT_BASE_INTERVAL);
	is_loaded = true;

	_boss = new BossScreen(
		new RTexture(texturesScene_game[4]),
		new RTexture(texturesScene_game[5]),
		GetBossViewport());
}

void SceneGame::OnFree()
{
	delete _boss;

	/*deallocate assets*/
	for (std::vector<Enemy*>::iterator enemy = _enemys.begin(); enemy != _enemys.end(); ++enemy) {
		delete *enemy;
	}
	_enemys.clear();
	delete[] _arrayShadow;
	delete[] _arrayVisibility;
	_arrayShadow = NULL;

	//Destroy textures???
	map->ResetDoorsOpen();

	globalAudios[HEARTBEAT].res.sound->stop();

	EngineInst->unLoadResources(texturesScene, texturesSceneSize);

}

/* update positions; delete if needed*/
void SceneGame::updateFireballs(int timems)
{
	for (std::list<Fireball*>::iterator it = fireballs.begin(); it != fireballs.end();) {
		if ((*it)->updatePosition(map, timems)) {
			delete *it;
			it = fireballs.erase(it);
		} else {
			it++;
		}
	}
}

void SceneGame::updatePlayers(int timems)
{

	//const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	/*react on keys for both players*/
	if (EngineInst->input()->getState(INPUT_MOVE_DOWN)) {
		_player1->updateDirection(DIRECT_DOWN);
	}

	if (EngineInst->input()->getState(INPUT_MOVE_UP)) {
		_player1->updateDirection(DIRECT_UP);
	}

	if (EngineInst->input()->getState(INPUT_MOVE_LEFT)) {
		_player1->updateDirection(DIRECT_LEFT);
	}

	if (EngineInst->input()->getState(INPUT_MOVE_RIGHT)) {
		_player1->updateDirection(DIRECT_RIGHT);
	}

	if (EngineInst->input()->getState(INPUT_SHOOT)) {
		Fireball * fb = _player1->Shoot();
		if (fb)
			fireballs.push_back(fb);
	}

	if (EngineInst->input()->getState(INPUT_USE)) {
		_player1->placeTrap();
	}

	_player1->OnUpdate(timems);

}

void SceneGame::updateCamera()
{
	_camera.x = _player1->getPosX() - GetGameplayViewport().w / 2 + EngineInst->getTileSize() / 2;
	_camera.y = _player1->getPosY() - GetGameplayViewport().h / 2 + EngineInst->getTileSize() / 2;
}

void SceneGame::updateEnemies(int timems)
{
	/* update behaviors for each of enemies */
	for (std::vector<Enemy*>::iterator enemy = _enemys.begin(); enemy != _enemys.end(); ++enemy) {
		(*enemy)->OnUpdate(timems);
		(*enemy)->Chase(_player1);

		const AStarWay_t & way1 = (*enemy)->getWay();

		if (heartbeat_tempo == 0 && ((way1.size() != 0 && way1.size() < 10 ))){
			heartbeat_tempo = 50;
			globalAudios[HEARTBEAT].res.sound->setDelay(HEARTBEAT_MIN_INTERVAL);
		} else if (heartbeat_tempo != 0) {
			heartbeat_tempo--;
			if (heartbeat_tempo == 0)
				globalAudios[HEARTBEAT].res.
					sound->setDelay(HEARTBEAT_BASE_INTERVAL);
		}

		
	}
}

void SceneGame::OnUpdate(int timems)
{
	if (EngineInst->input()->getState(INPUT_GAME_RESET)) {
		level->resetCurrent();
		return;
	}

	globalAudios[HEARTBEAT].res.sound->update(timems);
	_boss->OnUpdate(timems);
	updatePlayers(timems);
	updateFireballs(timems);
	updateEnemies(timems);
	updateShadows();
	updateCamera();
}
#include<math.h>

float sqrf(float a) {
	return a * a;
}
void SceneGame::updateShadowsChr(const Character *ch)
{
	int radius = 9;
	int alfa,xx,yy;
	int idx;
	int centerTiltX = ch->getPosAfterX();
	int centerTiltY = ch->getPosAfterY();
	ch->OnRenderCircle(radius);
	for(int y=-radius; y<=radius; ++y) {
		for(int x=-radius; x<=radius; ++x) {
			alfa = 255*(1-sqrf(
				    sqrtf(x*x + y*y) /
				    sqrtf(radius * radius *2)));
			xx = x + centerTiltX;
			yy = y + centerTiltY;
			if(alfa > 0 && ch->canSee(xx,yy)) {
				if(xx >= 0 && xx < _arrayShadowW
				   && yy >= 0 && yy < _arrayShadowH) {
					idx = yy*_arrayShadowW + xx;
					_arrayShadow[idx] += alfa;
					_arrayVisibility[idx] = true;
					if(_arrayShadow[idx] > 255) {
						_arrayShadow[idx] = 255;
					}
				}
			}
		}
	}
}

void SceneGame::updateShadows()
{
	memset(_arrayShadow, 00, _arrayShadowW * _arrayShadowH
	       * sizeof(_arrayShadowH));
#ifdef DEBUG_BOTS
	for (auto enemy : _enemys) {
		updateShadowsChr(enemy);
	}
#else
	for (auto enemy : _enemys) {
		enemy->OnRenderCircle(9);
	}
	
	updateShadowsChr(_player1);
#endif


	for (int i = 0 ; i != map->GetHeight(); i++) {
		for (int j = 0 ; j != map->GetWidth(); ++j) {
			int field = map->GetFieldAt(j, i)->GetType();
			//int tile =  map->GetFieldAt(j, i)->GetTileId();
			if (field == IField::MEDKIT ||  field == IField::POWERUP) {
				//updateShadowsObj4(j, i);
			}

		}
	}

}

void SceneGame::renderShadow(SDL_Renderer* renderer) {

	int tileSize = EngineInst->getTileSize();
	int alfa;

	for (int y = 0 ; y < _arrayShadowH; ++y) {
		for (int x = 0 ;x< _arrayShadowW; ++x) {
			alfa = 255 - _arrayShadow[y*_arrayShadowW + x];
			if (alfa > map->getParams()->alpha)
			{
				alfa = map->getParams()->alpha;
			}
			if (!_arrayVisibility[y*_arrayShadowW + x]) {
				#ifdef DEBUG_BOTS
				_tiles->setAlpha(190);
				#else
				_tiles->setAlpha(255);
				#endif

			} else {
				_tiles->setAlpha(alfa);
			}
			_tiles->renderTile(renderer,
					   x*tileSize - _camera.x, y*tileSize - _camera.y,
					   35, SDL_FLIP_NONE);
		}
	}

	_tiles->setAlpha(255);

}

static unsigned long _next = 1;

static int _rand(void)
{
	_next = _next * 1103515245 + 12345;
	return((unsigned)(_next/65536) % 32768);
}

void _srand(unsigned seed) {
	_next = seed;
}

void SceneGame::renderMap(SDL_Renderer* renderer) {
	
	int tileSize = EngineInst->getTileSize();

	_srand(1);

	/*Render background*/
	for (int i = 0 ; i != map->GetHeight(); i++) {
		for (int j = 0 ; j != map->GetWidth(); ++j) {
			int px_left = j * tileSize - _camera.x;
			int px_top  = i * tileSize - _camera.y;
			_tiles->renderTile(renderer,
			                   px_left,
			                   px_top,
			                   18 + _rand() % 5, SDL_FLIP_NONE);
		}
	}

	/*Render actual mapiles*/
	for (int i = 0 ; i != map->GetHeight(); i++) {
		for (int j = 0 ; j != map->GetWidth(); ++j) {
			int field = map->GetFieldAt(j, i)->GetType();
			int tile =  map->GetFieldAt(j, i)->GetTileId();
			if (field == IField::FLOOR)
				continue;
			int col = j * tileSize - _camera.x;
			int row = i * tileSize - _camera.y;
			_tiles->renderTile(renderer, col , row, tile, SDL_FLIP_NONE);

		}
	}
}


void SceneGame::OnRender(SDL_Renderer* renderer)
{
	renderGameplay(renderer);
	renderBoss(renderer);
	renderGUI(renderer);
}

void SceneGame::renderGameplay(SDL_Renderer *renderer)
{
	int tileSize = EngineInst->getTileSize();

	SDL_Rect topLeftViewport = GetGameplayViewport();
	SDL_RenderSetViewport(renderer, &topLeftViewport);
	
	renderMap(renderer);

	/* render enemies */
	for (std::vector<Enemy*>::iterator enemy = _enemys.begin(); enemy != _enemys.end(); ++enemy) {
		if ((*enemy)->GetState() == Character::ALIVE)
			(*enemy)->OnRender(renderer, &_camera);
	}

	/* render fireballs */
	for (std::list<Fireball*>::iterator it = fireballs.begin();
	it != fireballs.end(); ++it) {
		int sprite;
		if ((*it)->GetPowerLevel() > 30)
			sprite = 29;
		else
			sprite = 28;

		_tiles->renderTile(renderer, (*it)->getPosX() - _camera.x, (*it)->getPosY() - _camera.y, sprite, SDL_FLIP_NONE);
	}

	_player1->OnRender(renderer, &_camera);

	renderShadow(renderer);
}

void SceneGame::renderBoss(SDL_Renderer* renderer)
{
	SDL_Rect topLeftViewport = GetBossViewport();
	SDL_RenderSetViewport(renderer, &topLeftViewport);
	_boss->OnRender(renderer);
}

void SceneGame::renderGUI(SDL_Renderer *renderer) {
	// Render top bar
	int screenWidth = EngineInst->screen_width();
	int screenHeight = EngineInst->screen_width();
	int tileSize = EngineInst->getTileSize();

	SDL_Rect veryTopBar = GetUIViewport();
	
	int playerBarYPadding = 5;
	int playerBarXPadding = 20;
	int playerBarHeight = 20;
	int paddingBetweenBars = 5;

	SDL_RenderSetViewport(renderer, &veryTopBar);

		int defaultX = screenWidth - tileSize - playerBarXPadding;

		_player1->renderAvatar(renderer, defaultX, 0, SDL_FLIP_HORIZONTAL);
		EngineInst->font()->printf(defaultX - 210, playerBarYPadding + veryTopBar.y, ALIGN_RIGHT | ALIGN_TOP, "Player controls UP, DOWN, LEFT, RIGHT CTRLl-Fire");

	drawBar(renderer, _player1->getHealth(), playerBarYPadding, playerBarHeight, defaultX, 255, 0, 0);
	EngineInst->font()->printf(0, 0, ALIGN_LEFT | ALIGN_TOP, "Room %u-%u", level->getId(), room_id);

	playerBarYPadding+=paddingBetweenBars;
	playerBarYPadding+=playerBarHeight;
	drawBar(renderer, _player1->getMana(), playerBarYPadding, playerBarHeight, defaultX, 0, 0, 255);

	veryTopBar.h = screenHeight;
	SDL_RenderSetViewport(renderer, &veryTopBar);

	/* check loss condition */
	if (_player1->GetState() == Character::DEAD) 
	{
		EngineInst->setStatusLine("You lost! "
			"Press R to try again");
	}
	/*Check victory condition*/
	else if (_player1->GetState() == Character::WON) 
	{
		int target_level1 = level->getId() + 1;
		int target_map1 = 0;
		//int target_level2=level->getId()+1;
		//int target_map2=0;
		/* game ended with victory ? */
		bool game_end = false;
		Door * dor = dynamic_cast <Door*>(
			map->GetFieldAt(_player1->getPosAfterX(),
				_player1->getPosAfterY()));
		if (dor) {
			target_level1 = level->getId();
			target_map1 = dor->GetTargetBoard();
		}

		Stairs * sta = dynamic_cast <Stairs*>(
			map->GetFieldAt(_player1->getPosAfterX(),
			_player1->getPosAfterY())
		);

		if (sta && sta->GetVictory()) {
			game_end = true;
			EngineInst->clearStatusLine();
		}

		EngineInst->font()->printfLT(100,
			map->GetHeight()*tileSize, "Both players won");
		if (game_end) {
			level->SetVictoryScene();
		}
		else {
			level->setId(target_level1);
			level->setCurrentScene(target_map1);
		}
	}
}

void SceneGame::drawBar(SDL_Renderer *renderer, int value, int playerBarYPadding, int playerBarHeight, int defaultX, int r, int g, int b) const {
	SDL_Rect rect = {defaultX - value * 2, playerBarYPadding, value * 2, playerBarHeight};
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
	//Frame
	SDL_SetRenderDrawColor(renderer, dull(r), dull(g), dull(b), SDL_ALPHA_OPAQUE);
	rect.x = defaultX - 200;
	rect.w = 200;
	SDL_RenderDrawRect( renderer, &rect );
	rect.x++;
	rect.y++;
	rect.w -= 2;
	rect.h -= 2;
	SDL_RenderDrawRect( renderer, &rect );
}

int SceneGame::dull(int n) const {
	int dullOffset = 30;
	if(n>128+dullOffset)
		n-=dullOffset*4;
	if(n<128-dullOffset)
		n+=dullOffset;
	return n;
}
