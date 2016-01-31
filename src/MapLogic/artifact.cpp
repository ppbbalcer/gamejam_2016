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
	if (glowing) {
		glow_tick_timer -= time;
		if (glow_tick_timer < 0) {
			tile++;
			if (tile > ARTIFACT_TILE_END) {
				tile = ARTIFACT_TILE_BEGIN;
				glowing = 0;
			}
			glow_tick_timer = ARTIFACT_GLOW_TICK;
		}
	} else {
		glow_delay_timer -= time;
		if (glow_delay_timer < 0) {
			glowing = 1;
			glow_delay_timer = ARTIFACT_GLOW_DELAY;
		}
	}
}

int Artifact::GetTileId()
{
	return tile;
}
