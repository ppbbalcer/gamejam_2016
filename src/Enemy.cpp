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
	default_dir_x = -1;
	default_dir_y = 0;
	view_dir_x = -1;
	view_dir_y = 0;
	is_praying = true;
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
/* looking for obstacles*/
bool I_Map_isObstacle(int x, int y, void* objMap)
{
	if (((IMap*)objMap)->GetFieldAt(x, y)->IsOccupied())
		return false;
	return ((IMap*)objMap)->GetFieldAt(x, y)->IsObstacle();
}

void Enemy::Chase(Character * ch)
{

	if(getWay().size() > 0) {
		if ( canSee(ch->getPosBeforeX(),
			    ch->getPosBeforeY()))
		{
			if (getWayAge() < 1) {
				return;
			} else {
				puts("He runs away!");
			}
		} else {
			return;
		}
	} else if (!canSee(ch->getPosBeforeX(),
			   ch->getPosBeforeY())
		)
	{
		if (getWayAge() >1) {
			//Where is he?
			if (getPosBeforeY() == getPosAfterY()
			    &&
			    getPosBeforeX() == getPosAfterX()) {
				DIRECT d = getRandomDirection();
				if (d != DIRECT_NO_WAY) { 
					updateDirection(d);
				}
			}
		}
		return;
	}	
	puts("It's him!");
	
	
	int startX = getPosAfterX();
	int startY = getPosAfterY();
	AStarWay_t way1;
	
	int maxSteps = 0;
	DIRECT destBest = DIRECT_NO_WAY;
	int distX = ch->getPosX() - getPosX();
	int distY = ch->getPosY() - getPosY();
	
	
	if (ch->GetState() == Character::DEAD) {
		return;
	} else {
		destBest =
			findAstar(way1, maxSteps,
				  startX, startY,
				  ch->getPosBeforeX(),
				  ch->getPosBeforeY(),
				  _map->GetWidth(),
				  _map->GetHeight(),
				  I_Map_isObstacle, _map);
	}
	
	if (destBest != DIRECT_NO_WAY) {
		if (destBest == DIRECT_DOWN) {
			updateDirection(DIRECT_DOWN);
		} else if (destBest == DIRECT_UP) {
			updateDirection(DIRECT_UP);
		} else if (destBest == DIRECT_LEFT) {
			updateDirection(DIRECT_LEFT);
		} else if (destBest == DIRECT_RIGHT) {
			updateDirection(DIRECT_RIGHT);
		}
		setWay(way1);
	}
}

void Enemy::PrayToMonster()
{

	if (getPosBeforeX() > 3 && (way.empty() || wayAge > 1)) {
		
		DIRECT destBest = DIRECT_NO_WAY;
		int startX = getPosAfterX();
		int startY = getPosAfterY();
			AStarWay_t way1;
			int maxSteps = 0;

		destBest =
			findAstar(way1, maxSteps,
				  startX, startY,
				  1,
				  getPosBeforeY(),
				  _map->GetWidth(),
				  _map->GetHeight(),
				  I_Map_isObstacle, _map);

		if (destBest != DIRECT_NO_WAY && way1.size()) {
			if (destBest == DIRECT_DOWN) {
			updateDirection(DIRECT_DOWN);
			} else if (destBest == DIRECT_UP) {
				updateDirection(DIRECT_UP);
			} else if (destBest == DIRECT_LEFT) {
				updateDirection(DIRECT_LEFT);
			} else if (destBest == DIRECT_RIGHT) {
				updateDirection(DIRECT_RIGHT);
			}
			setWay(way1);
		}
	} else {

		view_dir_x = -1;
		view_dir_y = 0;
		last_dir_x = -1;
		last_dir_y = 0;
		default_dir_x = view_dir_x;
		default_dir_y = view_dir_y;
		_texture->setFlip(SDL_FLIP_NONE);
		if (wayAge > 1) {
			wayAge = 0;
			puts("Wololo!");
			// this means by default one cultist can summon
			// ctholhoo in 40 seconds. game lasts 50 seconds
			// in the demo level

			_map->ProgressMonster((1.f/_map->getParams()->summon_seconds) * _map->getParams()->cultist_seconds);
		}
	}
}
void Enemy::ProcessAI(Character * ch, int time_ms)
{
	/* enemy can be in two states.
	 * either praying to his monster
	 * or chasing PC.
	 */
	if (is_praying) {
		if ( canSee(ch->getPosBeforeX(),
			    ch->getPosBeforeY())) {
			is_praying = false;
			chase_took = 0;
			puts("AI:Chase");
		}
	} else {
		if (chase_took > 15) {
			way.clear();
			is_praying = true;
			puts("AI: Pray");
			
		}
	}
	if (is_praying) {
		PrayToMonster();
	} else {
		Chase(ch);
	if (!is_praying && !canSee(ch->getPosBeforeX(),
				   ch->getPosBeforeY())) {
		chase_took += time_ms * 0.001;
	}		
	}

}

void Enemy::OnUpdate(int time_ms)
{
	Character::OnUpdate(time_ms);
	wayAge += time_ms*0.001;
	if (getPosAfterX() == getPosBeforeX()
	    && getPosAfterY() == getPosBeforeY()
	    && way.empty() && is_praying) {
		std::vector<int> w;
		w.push_back(1);
		w.push_back(2);
		AnimateFrames(time_ms,w);
	} else {

		if (view_dir_x == 0 && view_dir_y < 0) {
			std::vector<int > w;
			w.push_back(6);
			w.push_back(7);
			AnimateFrames(time_ms,w);
		} else if (view_dir_x == 0 && view_dir_y) {
			std::vector<int > w;
			w.push_back(8);
			w.push_back(9);
			AnimateFrames(time_ms,w);
		} else {
			std::vector<int > w;
			w.push_back(3);
			w.push_back(4);
			w.push_back(5);
			w.push_back(4);
			AnimateFrames(time_ms,w);
		}
	}
	if (!way.empty()) {
		AStartPoint_t next = *way.begin();
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
			} else {
				wayAge = 0;
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
			if (view_dir_y > 0 & j < getPosBeforeY()) continue;
			if (view_dir_y < 0 & j > getPosBeforeY()) continue;
			if (view_dir_x > 0 & j < getPosBeforeX()) continue;
			if (view_dir_x < 0 & j > getPosBeforeX()) continue;
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
	Character::updateDirection(directMove);
	default_dir_x = view_dir_x;
	default_dir_y = view_dir_y;
	if (_map->GetFieldAt(getPosAfterX()+default_dir_x,
			     getPosAfterY()+default_dir_y)
	    ->IsObstacle()) {
		default_dir_x = default_dir_y = 0;
	}
}

DIRECT Enemy::getRandomDirection()
{
	//clock_t now = clock();
	wayAge = 0.0f;
	if (_time_to_random_direction) {
		return DIRECT_NO_WAY;
	}
	if (_map->GetFieldAt(getPosAfterX()+default_dir_x,
			     getPosAfterY()+default_dir_y)
	    ->IsObstacle()) {
		default_dir_x = default_dir_y = 0;
	}
	if ((default_dir_y || default_dir_x) && (rand() % 10 > 3)) {
		if (default_dir_y > 0) {
			return DIRECT_DOWN;
		} else if (default_dir_y < 0) {
			return DIRECT_UP;
		} else if (default_dir_x < 0) {
			return DIRECT_LEFT;
		} else {
			return DIRECT_RIGHT;
		}
	} else {
		DIRECT result = (static_cast<DIRECT>((rand() % (DIRECT_END - 1)) + 1));
		if (result == DIRECT_LEFT
		    && !_map->GetFieldAt(getPosAfterX()-1,
			     getPosAfterY())
		    ->IsObstacle()) {
			return result;
		} else if (result == DIRECT_RIGHT
		    && !_map->GetFieldAt(getPosAfterX()+1,
			     getPosAfterY())
		    ->IsObstacle()) {
			return result;
		} else if (result == DIRECT_DOWN
		    && !_map->GetFieldAt(getPosAfterX(),
			     getPosAfterY()+1)
		    ->IsObstacle()) {
			return result;
		} else if (result == DIRECT_UP
		    && !_map->GetFieldAt(getPosAfterX(),
			     getPosAfterY()-1)
		    ->IsObstacle()) {
			return result;
		} else {
			return getRandomDirection();
		}
	}
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
//#define DEBUG_BOTS

void Enemy::OnRender(SDL_Renderer *renderer, SDL_Point *camera)
{
	//For debug A*
#ifdef DEBUG_BOTS
	{ //Astar Example
		int tileSize = EngineInst->getTileSize();
		for(std::list<AStartPoint_t>::iterator step = way.begin(); step != way.end(); ++step) {
			int x = (*step).first;
			int y = (*step).second;
			EngineInst->font()->printfLT(x*tileSize, y*tileSize, "X");
			_texture->renderTile(renderer, x*tileSize, y*tileSize);

		}
	}
#endif
	Character::OnRender(renderer, camera);
}
