#include "../Character.h"
#include "field.h"
#include "field_impl.h"
#include "artifact.h"

void Artifact::SteppedOver(Character * who)
{
	Field::SteppedOver(who);
}

void Artifact::update(int time)
{

}

int Artifact::GetTileId()
{
	return 58;
}
