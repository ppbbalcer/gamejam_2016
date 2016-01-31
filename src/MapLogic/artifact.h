#ifndef __ARTIFACT_FIELD_H
#define __ARTIFACT_FIELD_H
#include <list>
#include "field.h"

#define ARTIFACT_GLOW_TICK 100
#define ARTIFACT_GLOW_DELAY 1000

#define ARTIFACT_TILE_BEGIN 56
#define ARTIFACT_TILE_END 59

class Artifact: public Field
{
public:
	void SteppedOver(Character * who);

	Artifact() : Field(ARTIFACT)
	{
		tile = ARTIFACT_TILE_BEGIN;
		glow_tick_timer = 0;
		glow_delay_timer = 0;
		glowing = 0;
	}

	virtual void update(int time);
	virtual int GetTileId();

	int tile;
	int glowing;
	int glow_tick_timer;
	int glow_delay_timer;
};

#endif
