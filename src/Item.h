//
// Created by zbyshekh on 1/30/16.
//

#ifndef BASE_GAME_ITEM_H
#define BASE_GAME_ITEM_H


class Item {
public:
	Item(bool stackable, int maxAmmo);
	bool stackable;
	int ammo;
	int maxAmmo;
	bool available;
	bool use();
	void pickupAmmo(int);
	void pickupAmmo();

};


#endif //BASE_GAME_ITEM_H
