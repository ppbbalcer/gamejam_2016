#include "field.h"
#include <cassert>
#include "field_impl.h"
#include "../Player.h"
#include "../GlobalData.h"
#include "skull.h"

int Field::assigned_fieldID[NUM_FIELD_TYPES];
int Field::assigned_fieldGroupId[NUM_FIELD_TYPES];

void Field::EnsureFieldIdsInitialized()
{
	static bool initialized = false;
	if (initialized) {
		return;
	}
	assigned_fieldID[WALL_HORIZONTAL] = 5;

	assigned_fieldID[WALL_VERTICAL] = 13;
	assigned_fieldID[WALL_CROSS] =  02;
	assigned_fieldID[WALL_LT] = 3;
	assigned_fieldID[WALL_RT] = 9;
	assigned_fieldID[WALL_LB] = 1;
	assigned_fieldID[WALL_RB] = 4;
	assigned_fieldID[T_LEFT] =  6;
	assigned_fieldID[T_RIGHT] = 11;
	assigned_fieldID[T_TOP] =  12;
	assigned_fieldID[T_BOTTOM] =  10;
	assigned_fieldID[FLOOR] = 7;
	assigned_fieldID[DESK] = 7;
	assigned_fieldID[DOOR] = 7;
	assigned_fieldID[DOOR_VERTICAL_CLOSED] = 17;
	assigned_fieldID[DOOR_VERTICAL_OPEN] = 16;
	assigned_fieldID[DOOR_HORIZONTAL_CLOSED] = 14;
	assigned_fieldID[DOOR_HORIZONTAL_OPEN] = 15;
	assigned_fieldID[MEDKIT] = 25;
	assigned_fieldID[INACTIVE_SKULL] = 26;
	assigned_fieldID[SILVER_SKULL] = 45;
	assigned_fieldID[GOLDEN_SKULL] = 44;
	assigned_fieldID[POWERUP] = 31;
	assigned_fieldID[SMALL_MANA_FLASK] = 36;
	assigned_fieldID[LARGE_MANA_FLASK] = 40;
	assigned_fieldID[SMALL_HEALTH_FLASK] = 34;
	assigned_fieldID[LARGE_HEALTH_FLASK] = 33;
	assigned_fieldID[EMPTY_FLASK] = 32;
	assigned_fieldID[SWITCH_DOWN] = 38;
	assigned_fieldID[SWITCH_UP] = 37;
	assigned_fieldID[ROOT_TRAP] = 30;
	assigned_fieldID[TRAP] = 30;
	assigned_fieldID[STAIRS] = 46;
	assigned_fieldID[SPIKES_ON] = 47;
	assigned_fieldID[SPIKES_OFF] = 48;
	assigned_fieldID[EVIL_COMPUTER] = 39;
	assigned_fieldID[ARTIFACT] = 39;
	initialized = true;
}

void Field::EnsureFieldGroupIdsInitialized(){
	
	static bool initializedGroupIds = false;
	if (initializedGroupIds) {
		return;
	}
	
	// assign all to 0th group first
	for(int i=0; i<NUM_FIELD_TYPES; i++){
		assigned_fieldGroupId[i] = 0;
	}

	assigned_fieldGroupId[WALL] = 2;
	assigned_fieldID[WALL_HORIZONTAL] = 2;
	assigned_fieldID[WALL_VERTICAL] = 2;
	assigned_fieldID[WALL_CROSS] =  2;
	assigned_fieldID[WALL_LT] = 2;
	assigned_fieldID[WALL_RT] = 2;
	assigned_fieldID[WALL_LB] = 2;
	assigned_fieldID[WALL_RB] = 2;

	initializedGroupIds = true;

}

int Field::GetTileId()
{
	EnsureFieldIdsInitialized();
	return assigned_fieldID[GetType()];
}

/** this is necessary since there are more than one tile textures (groups) which are used */
int Field::GetTileGroupId()
{
	EnsureFieldGroupIdsInitialized();

	return assigned_fieldGroupId[GetType()];
}

bool Field::IsObstacle()
{
	if (someone_is_here) {
		return true;
	}
	if (GetType() == DOOR_VERTICAL_CLOSED ||
	                GetType() == DOOR_HORIZONTAL_CLOSED)
		return true;
	return (type >= WALL && type <= T_BOTTOM);
}

int Field::GetType()
{
	return type;
}

void Field::SteppedOver(Character * who)
{
	if (type == POWERUP) {
		type = FLOOR;
		who->SetPowerLevel(who->GetPowerLevel() + POWERUP_VAL);
	}
	if (type == LARGE_HEALTH_FLASK && who->getType() == TYPE_PLAYER) {
		type = EMPTY_FLASK;
		who->heal(LARGE_POTION_HEAL_VAL);
		globalAudios[SLURP].res.sound->play();
	}
	if (type == SMALL_HEALTH_FLASK && who->getType() == TYPE_PLAYER) {
		type = EMPTY_FLASK;
		who->heal(SMALL_POTION_HEAL_VAL);
		globalAudios[SLURP].res.sound->play();
	}
	if (type == SMALL_MANA_FLASK && who->getType() == TYPE_PLAYER) {
		type = EMPTY_FLASK;
		Player *player = (Player *)who;
		player->restoreMana(SMALL_POTION_MANA_VAL);
		globalAudios[SLURP].res.sound->play();
	}
	if (type == LARGE_MANA_FLASK && who->getType() == TYPE_PLAYER) {
		type = EMPTY_FLASK;
		Player *player = (Player *)who;
		player->restoreMana(LARGE_POTION_MANA_VAL);
		globalAudios[SLURP].res.sound->play();
	}
	if (type == MEDKIT) {
		type = FLOOR;
		who->heal(MEDKIT_HEAL_VAL);
	}
	someone_is_here = who;
}

void Field::LeftField()
{
	someone_is_here = 0;
}

Character* Field::WhoIsHere()
{
	return someone_is_here;
}
