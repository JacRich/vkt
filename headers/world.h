#ifndef VENG_H
#define VENG_H

#include "globaldef.h"
#include "ivec.h"
#include "vhit.h"

#include "player.h"
#include "region.h"

#include "window.h"

#define PICK_HIT    0
#define PICK_NORMAL 1


void world_init();
void world_tick();
void world_terminate();

void world_change(vhit voxel, int pickmode, uint8 value);
void world_change_range(int cubicRange, vec pos, uint8 newValue, int filter);

region_t* world_find_region(vec worldpos);
chunk_t* world_find_chunk(vec worldpos);

vhit world_find_voxel(vec pos);

vhit world_raycast(int range, vec pos, vec dir);

#endif 