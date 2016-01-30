#include "../Character.h"
#include "field.h"
#include "field_impl.h"
#include "root_trap.h"

#define ROOT_TIMER 2000

void RootTrap::SteppedOver(Character * who)
{
	if (who->getType() == TYPE_ENEMY) {
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
