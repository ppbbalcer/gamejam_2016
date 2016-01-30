//
// Created by zbyshekh on 1/30/16.
//

#include "Item.h"

bool Item::use() {
	if(available && ammo){
		ammo--;
		return true;
	}
	return false;
}

void Item::pickupAmmo(int i) {
	ammo+=i;
	if(ammo>maxAmmo)
		ammo = maxAmmo;
}

void Item::pickupAmmo() {
	pickupAmmo(1);
}

Item::Item(bool stackable, int maxAmmo) {
	this->stackable = stackable;
	this->maxAmmo = maxAmmo;
	ammo = 0;
	available = false;
}
