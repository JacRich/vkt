#ifndef CHUNK_H
#define CHUNK_H

#include "globaldef.h"

using voxel_t = uint8;

// Chunks seems to just be something to attach chunk meshes to, maybe
// this whole thing needs a rework?
struct chunk_t
{
  voxel_t voxels[VOXELS_CROOT][VOXELS_CROOT][VOXELS_CROOT];
  vec pos = {0.0f, 0.0f, 0.0f};

  bool update = false;
};

enum VOXELTYPES
{
  V_STONE_BLACK = (voxel_t)1,
  V_STONE_GREY = 2,
  V_STONE_ORANGE = 3,
  V_STONE_PEPPER = 4
};

void chunk_fill_perlin(chunk_t *chunk);

void chunk_fill(chunk_t *chunk, voxel_t value);

#endif
