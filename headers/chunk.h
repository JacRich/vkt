#ifndef CHUNK_H
#define CHUNK_H
#include "globaldef.h"
#include "ivec.h"

struct chunk_t
{
  uchar voxels [CHUNK_CROOT][CHUNK_CROOT][CHUNK_CROOT];
  vec   pos  = {0.0f, 0.0f, 0.0f}; 

  bool update  = false;
};

enum VOXELTYPES
{
  V_STONE_BLACK  = 1,
  V_STONE_GREY   = 2,
  V_STONE_ORANGE = 3,
  V_STONE_PEPPER = 4
};

void chunk_fill_perlin(chunk_t* chunk);
void chunk_fill(chunk_t* chunk, uchar value);

#endif
