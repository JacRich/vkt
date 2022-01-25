#ifndef CHUNK_H
#define CHUNK_H
#include "globaldef.hpp"
#include "ivec.hpp"

struct chunk_t
{
  uchar voxels [32][32][32];
  vec   pos  = {0.0f, 0.0f, 0.0f};
  ivec  cord = {0, 0, 0}; 

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
