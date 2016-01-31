#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Character.h"
#include <time.h>
#include "Engine/AStar.h"

#define RANDOM_DIRECTION_CHANGE_TIME_MS 500

enum enemy_ai {
	ENEMY_AI_UNKNOWN,
	ENEMY_AI_DEFAULT,
	ENEMY_AI_OFF,
	ENEMY_AI_DISTANCE
};

/**
 * class represents non-player characters
 */
class Enemy : public Character
{
	//! in milliseconds
	clock_t _time_to_attack;
	clock_t _time_to_random_direction;
	//! ai instance
	enemy_ai _ai;
	AStarWay_t way;
	float wayAge;
	bool is_praying;
public:
	Enemy(RTexture* texture, IMap * map, int hp, int ai);
	virtual ~Enemy(void);
	virtual void OnRender(SDL_Renderer *renderer, SDL_Point *camera);
	float getWayAge() {return wayAge;}
	virtual void OnUpdate(int time_ms);
	enemy_ai getAI() { return _ai; }
	virtual int crucio(int howMuchCrucio);
	DIRECT getRandomDirection();
	void				setWay(AStarWay_t& pway);
	const AStarWay_t&	getWay();
	virtual void updateDirection(DIRECT directMove);
	void Chase(Character * ch);
	void ProcessAI(Character * ch, int timems);
	void PrayToMonster();
private:
	// time spent on chasing player WHEN player is not seen
	float chase_took;
	int default_dir_x;
	int default_dir_y;
};

#endif
