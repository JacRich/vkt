#ifndef VHIT_H
#define VHIT_H

#include "ivec.h"
#include "chunk.h"

#define HIT_NULL  -1
#define HIT_FALSE  0
#define HIT_TRUE   1
#define HIT_ALL    2

struct vhit 
{
  int state = HIT_NULL;

  voxel_t* voxel     = NULL;
  voxel_t* voxelLast = NULL;

  chunk_t* chunk     = NULL;
  chunk_t* chunkLast = NULL;

  vec pos;
  vec posLast;

  float distance = 0.0f;
};

#endif
