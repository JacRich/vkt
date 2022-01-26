#ifndef REGION_H
#define REGION_H
#include "globaldef.hpp"
#include "chunk.hpp"
#include "ivec.hpp"

struct region_t
{
  bool inUse = false;
  vec  pos   = {0,0,0};
  ivec cord  = {0,0,0};
  chunk_t chunks[REGION_CROOT][REGION_CROOT][REGION_CROOT];
};


void regions_cord_to_fill(region_t regions[REGION_COUNT], ivec cords[REGION_COUNT]);
bool is_cord_empty(region_t regions[REGION_COUNT], ivec cord);


void region_fill_perlin(region_t *region);
void region_fill_value(region_t *region, uchar value);
void region_set_pos(region_t* region, ivec cord);
void region_save(region_t* region);
void region_load(region_t* region);

#endif