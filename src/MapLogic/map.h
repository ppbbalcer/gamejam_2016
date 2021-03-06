#ifndef __MAP_H
#define __MAP_H
#include "field.h"
#include <utility>
#include <list>

struct enemy_definition {
	int x;
	int y;
	int hp;
	int ai;
};

struct map_params {
	int alpha;
	int start_mana;
	int start_hp;
	int start_ammo;
	int start_trap;
	int day_seconds;
	int cultist_seconds;
	int summon_seconds;
};

enum map_obstruction {
	OBSTRUCTION_TRAP,

	MAX_MAP_OBSTRUCTIONS
};

typedef std::pair<int,int> starting_pos;
typedef std::list<starting_pos> start_list;
typedef std::list<enemy_definition*> enemies_list;

class IMap {
  
public:
	enum MapType {
		GENERATED=1, // not implemented
		LOADED=2
	};
	/**
	 * @param type type of map to be factored (as per enum MapType)
	 * @param parameter - parameter as per map time. probably a path
	 * @return pointer to a map. If failed to generate, 0 is returned
	 *   and an exception is thrown
	 */
	static IMap *Factory(int type, const char *parameter);

	virtual const starting_pos & GetPlayer1Start() =0;
	virtual const starting_pos & GetPlayer2Start() =0;
	virtual const enemies_list & GetEnemies() = 0;
	virtual map_params* getParams() = 0;
	virtual int placeObstruction(int x, int y, map_obstruction ob) = 0;

	/**
	 * @return in fields
	 */
	virtual int GetWidth() = 0;
	/**
	 *
	 * @return in fields
	 */
	virtual int GetHeight() = 0;
	/**
	 * @param x,y field position (0-n)
	 */
	virtual IField* GetFieldAt(int x, int y) = 0;
	virtual const char *GetTitleString() = 0;
	virtual void ResetDoorsOpen() = 0;
	virtual void SetDoorsOpen(bool open) = 0;
	virtual bool GetDoorsOpen() = 0;

	/**
	 * retrieve current progress of dawn
	 * (0 means start of the game, 1 means it's bright out there)
	 */
	virtual float GetDayProgress() = 0;
	
	/**
	 * retrieve current progress of monster summoning
	 * (0 means, monster is NOT present, 1 means, he is summoned
	 * fully and cultists won */
	virtual float GetMonsterProgress() = 0;
	
	/**
	 * advance progress of monster summoning by a fraction
	 */
	virtual void ProgressMonster(float prog) = 0;
	virtual void ProgressDay(float prog) = 0;

	virtual void update(int time) = 0;
	virtual void ResetConditions() = 0;
	virtual ~IMap() {};

};


#endif
