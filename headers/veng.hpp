#ifndef VENG_H
#define VENG_H
#include "globaldef.hpp"
#include "ivec.hpp"
#include "vhit.hpp"

#include "player.hpp"
#include "region.hpp"
#include "cursor.hpp"

#include "render.hpp"

#define PICK_HIT    0
#define PICK_NORMAL 1


void veng_init();
void veng_tick();
void veng_terminate();

void veng_change_voxel(vhit voxel, int pickmode, uchar value);
void veng_change_withcursor(cursor_t* cursor, uchar newValue, int collMode);

region_t* veng_find_region(vec worldpos);
chunk_t*  veng_find_chunk (vec worldpos);
vhit      veng_find_voxel (vec pos);
vhit      veng_raycast    (int range, vec pos, vec dir);

#endif 