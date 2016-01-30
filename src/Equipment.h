//
// Created by zbyshekh on 1/30/16.
//

#ifndef BASE_GAME_EQUIPMENT_H
#define BASE_GAME_EQUIPMENT_H

#include "Item.h"


class Equipment {
	public:
		Equipment();
		Item * pistol;
		Item * artifact;
		Item * trap;
		bool firePistol();
		bool setTrap();
		bool useArtifact();

};


#endif //BASE_GAME_EQUIPMENT_H
