#include "RTexture.h"
#include <unordered_map>
#include "../MapLogic/map.h"

#define MAX_HEALTH 100

class Character
{
public:
	enum Action {
		ACTION_MOVE_UP = 0,
		ACTION_MOVE_DOWN,
		ACTION_MOVE_RIGHT,
		ACTION_MOVE_LEFT,
		ACTION_USE,
		ACTION_SHOOT,
		ACTION_END
	};

	Character(RTexture *texture);
	~Character();

	int getHealth();
	int crucio(int howMuchCrucio);
	void heal(int howMuchHeal);

	void render(SDL_Renderer *renderer, RTexture *tiles);
	void setPosTiles(int x, int y);
	int getPosX();
	int getPosY();
	void updateDirection(IMap *map, Action action);
	void updatePosition(IMap *map, int time_ms);
	int getPosBeforeX();
	int getPosAfterX();
	int getPosBeforeY();
	int getPosAfterY();

private:
	void setPos(int x, int y);
	RTexture *_texture;
	int _health;
	int _speed;


	int _posX;
	int _posY;
	int _pos_before_x;
	int _pos_after_x;
	int _pos_before_y;
	int _pos_after_y;
};
