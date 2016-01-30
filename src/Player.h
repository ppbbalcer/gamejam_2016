#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Character.h"
#include <time.h>

enum inventory_item {
	ITEM_TRAP,
	ITEM_AMMO,

	MAX_INVENTORY_ITEM
};

class Player : public Character
{
private:
	int _mana;
	int _time_to_shot;
	struct {
		int available;
		int capacity;
	} inventory[MAX_INVENTORY_ITEM];

public:
	Player(RTexture* texture, IMap * map, int hp, int mana);
	~Player(void);
	virtual void OnUpdate(int time_ms);

	virtual void Win();
	int getMana();
	void restoreMana(int howMuchMana);
	virtual int crucio(int howMuchCrucio);

	void placeTrap();
	void modInventoryItemCount(inventory_item item, int mod);

	Fireball * Shoot();
};

#endif
