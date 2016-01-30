//
// Created by zbyshekh on 1/30/16.
//

#include "Equipment.h"

Equipment::Equipment(){
	pistol   = new Item(true, 9);
	artifact = new Item(true, 3);
	trap     = new Item(false, 0);
}

bool Equipment::firePistol() {
	return pistol->use();
}

bool Equipment::setTrap() {
	return trap->use();
}

bool Equipment::useArtifact(){
	if (artifact->available){
		artifact->available = false;
		return true;
	}
	return false;
}