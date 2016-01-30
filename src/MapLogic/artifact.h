#ifndef __ARTIFACT_FIELD_H
#define __ARTIFACT_FIELD_H
#include <list>
#include "field.h"

class Artifact: public Field
{
public:
	void SteppedOver(Character * who);

	Artifact() : Field(ARTIFACT)
	{
	}
};

#endif
