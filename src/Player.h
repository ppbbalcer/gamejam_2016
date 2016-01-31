#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Character.h"
#include <time.h>

enum inventory_item {
	ITEM_TRAP,
	ITEM_AMMO,

	MAX_INVENTORY_ITEM
};

enum inventory_value_type {
	ITEM_CURRENT,
	ITEM_CAPACITY
};

#define TRAP_ARM_TIME 1500

class Player : public Character
{
private:
	int _mana;
	int _time_to_shot;
	int _time_to_arm;
	int arming;
	struct {
		int available;
		int capacity;
	} inventory[MAX_INVENTORY_ITEM];

public:
	Player(RTexture* texture, IMap * map, int hp, int mana, int ammo, int traps);
	~Player(void);
	virtual void OnUpdate(int time_ms);
	virtual void OnRender(SDL_Renderer *renderer, SDL_Point *camera);
	virtual void Win();
	int getMana();
	void restoreMana(int howMuchMana);
	virtual int crucio(int howMuchCrucio);
	virtual void onDirectionUpdate();

	void placeTrap();
	void modInventoryItemCount(inventory_item item, int mod);

	int getInvetoryInfo(inventory_item item, inventory_value_type type);

	Fireball * Shoot();
};

#endif
