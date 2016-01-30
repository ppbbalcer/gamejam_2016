#include "Enemy.h"
#include "GlobalData.h"
#include "Player.h"
#include "Engine/Engine.h"
#include <algorithm>
using namespace std;
Enemy::Enemy(RTexture *texture, IMap *map, int hp, int ai) : Character(texture, map)
{
	_type = TYPE_ENEMY;
	_time_to_attack = 0;
	_time_to_random_direction = 0;
	_health = hp;
	_ai = (enemy_ai)ai;
	wayAge = 0.0f;
	default_dir_x = 0;
	default_dir_y = 0;
}

Enemy::~Enemy(void) {};

int Enemy::crucio(int howMuchCrucio)
{
	int hpRem = Character::crucio(howMuchCrucio);
	if (hpRem <= 0) {
		globalAudios[ENEMY_DEATH].res.sound->play();
	}
	return hpRem;
}

void Enemy::OnUpdate(int time_ms)
{
//	time_ms = 9;

	Character::OnUpdate(time_ms);

	wayAge += time_ms;
	
	if (!way.empty()) {
		AStartPoint_t & next = *way.begin();
		if (getPosAfterX() == getPosBeforeX()
		    && getPosAfterY() == getPosBeforeY())
		{

			if (next.first == getPosBeforeX()
			    && next.second == getPosBeforeY()) {
				way.pop_front();
			}
			if (!way.empty()) {
				next = *way.begin();
				if (next.first > getPosBeforeX()) {
					updateDirection(DIRECT_RIGHT);
				} else if (next.first < getPosBeforeX()) {
					updateDirection(DIRECT_LEFT);
				} else if (next.second < getPosBeforeY()) {
					updateDirection(DIRECT_UP);
				} else {
					updateDirection(DIRECT_DOWN);
				}
			}
		}
	}
				  
	// Don't attack PC if I am dead
	if (GetState()!=ALIVE)
		return;
	_time_to_attack=max<int>(_time_to_attack-time_ms,
			       0);
	_time_to_random_direction=max<int>(_time_to_random_direction-time_ms,
					   0);

	if (_time_to_attack)
		return;

	/* inflicit damage on all player characters on neighboring
	 * fields
	 */
	for (int i = getPosBeforeX()-1; i!= getPosBeforeX()+2; ++i)
		for (int j = getPosBeforeY()-1; j!= getPosBeforeY()+2; ++j)
		{
			/*check if field is within boundaries
			 * of the map
			 */
			if (i>=_map->GetWidth()) continue;
			if (i<0) continue;
			if (j>=_map->GetHeight()) continue;
			if (j<0) continue;
			Character * wih =
				_map->GetFieldAt(i,j)->WhoIsHere();
			/* if field is occupied and a character
			 * who is there happens to be a players
			 * character (don't harm AIs) */
			if (wih && dynamic_cast<Player*>(wih))
			{
				printf("Crucio!\n");
				wih->crucio(20);
			}
			
		}
	_time_to_attack=500;
}
void Enemy::updateDirection(DIRECT directMove)
{
	default_dir_x = view_dir_x;
	default_dir_y = view_dir_y;
	if (_map->GetFieldAt(getPosAfterX()+default_dir_x,
			     getPosAfterY()+default_dir_y)
	    ->IsObstacle()) {
		puts("ZERO");
		default_dir_x = default_dir_y = 0;
	}
	Character::updateDirection(directMove);
}

DIRECT Enemy::getRandomDirection()
{
	//clock_t now = clock();
	if (_time_to_random_direction) {
		return DIRECT_NO_WAY;
	}
	if (default_dir_y || default_dir_y) {
		if (default_dir_y > 0) {
			return DIRECT_DOWN;
		} else if (default_dir_y < 0) {
			return DIRECT_UP;
		} else if (default_dir_x < 0) {
			return DIRECT_LEFT;
		} else {
			return DIRECT_RIGHT;
		}
	}
	_time_to_random_direction= RANDOM_DIRECTION_CHANGE_TIME_MS;
	return (static_cast<DIRECT>((rand() % (DIRECT_END - 1)) + 1));
}

void Enemy::setWay(AStarWay_t& pway)
{
	wayAge = 0.0f;
	printf("SetWay %d \n", way.size());
	way = pway;
}

const AStarWay_t& Enemy::getWay()
{
	return way;
}

void Enemy::OnRender(SDL_Renderer *renderer)
{
	//For debug A*
	{ //Astar Example
		int tileSize = EngineInst->getTileSize();
		for(std::list<AStartPoint_t>::iterator step = way.begin(); step != way.end(); ++step) {
			int x = (*step).first;
			int y = (*step).second;
			//EngineInst->font()->printfLT(x*tileSize, y*tileSize, "X");
			_texture->renderTile(renderer, x*tileSize, y*tileSize);

		}
	}
	Character::OnRender(renderer);
}
