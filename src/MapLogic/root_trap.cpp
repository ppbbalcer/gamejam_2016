#include "../Character.h"
#include "field.h"
#include "field_impl.h"
#include "root_trap.h"

#define ROOT_TIMER 2000

void RootTrap::SteppedOver(Character * who)
{
	if (tile == 60 && who->getType() == TYPE_ENEMY) {
		tile = 61;
		who->root(ROOT_TIMER);
	}
	Field::SteppedOver(who);
}

void RootTrap::Activate()
{
}

void RootTrap::Deactivate()
{
}

int RootTrap::GetTileId()
{
	return tile;
}
