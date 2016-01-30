#ifndef __ROOT_TRAP_FIELD_H
#define __ROOT_TRAP_FIELD_H
#include <list>
#include "field.h"

class RootTrap: public Field
{
public:
	void Activate();
	void Deactivate();
	void SteppedOver(Character * who);

	RootTrap() : Field(ROOT_TRAP) { };
};

#endif
