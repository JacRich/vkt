#ifndef VENG_H
#define VENG_H
#include "globaldef.hpp"
#include "ivec.hpp"
#include "vhit.hpp"

#include "player.hpp"
#include "region.hpp"
#include "chunk.hpp"
#include "cursor.hpp"

#define PICK_HIT    0
#define PICK_NORMAL 1

enum VOXELTYPES
{
    V_BASAL  = 1,
    V_ANDE   = 2,
    V_GRAN   = 3,
    V_BIOTI  = 4
};

struct sector_t
{
    uchar size  = 0;
    uchar value = 0;
};

void veng_init();
vhit veng_sample_pos(vec pos);
vhit veng_raycast   (int range, vec pos, vec dir);
void veng_change_withcursor(cursor_t* cursor, uchar newValue, int collMode);
void veng_tick();
void veng_replace_tick(player_t* player);
void veng_change_voxel(int pickmode, vhit voxel, uchar value);
void veng_terminate();

region_t* find_region(vec pos);
void save_region(region_t* region);

#endif 