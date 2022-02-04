#ifndef VENG_H
#define VENG_H

#include "globaldef.h"
#include "ivec.h"
#include "vhit.h"

#include "player.h"
#include "region.h"

#include "render.h"

#define PICK_HIT    0
#define PICK_NORMAL 1


void veng_init();

void veng_tick();

void veng_terminate();

void veng_change_voxel(vhit voxel, int pickmode, uchar value);

void veng_change_range(int cubicRange, vec pos, uchar newValue, int filter);

region_t *veng_find_region(vec worldpos);

chunk_t *veng_find_chunk(vec worldpos);

vhit veng_find_voxel(vec pos);

vhit veng_raycast(int range, vec pos, vec dir);

#endif 