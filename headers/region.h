#ifndef REGION_H
#define REGION_H

#include "globaldef.h"
#include "chunk.h"
#include "ivec.h"

struct region_t {
    vec pos = {0, 0, 0};
    ivec cord = {0, 0, 0};
    chunk_t chunks[REGION_CHUNKS_CROOT][REGION_CHUNKS_CROOT][REGION_CHUNKS_CROOT];
};

void region_fill_perlin(region_t *region);

void region_fill_value(region_t *region, uchar value);

void region_set_pos(region_t *region, ivec cord);

region_t region_load(ivec cord);

void region_save(region_t *region);

#endif