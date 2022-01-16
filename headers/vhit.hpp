#ifndef VHIT_H
#define VHIT_H
#include "ivec.hpp"
#include "chunk.hpp"

#define HIT_NULL  -1
#define HIT_FALSE  0
#define HIT_TRUE   1
#define HIT_ALL    2

struct vhit
{
    int state = HIT_NULL;
    
    uchar* voxel     = NULL;
    uchar* voxelLast = NULL;
    
    chunk_t* chunk     = NULL;
    chunk_t* chunkLast = NULL;

    vec pos    ;
    vec posLast;
};

#endif
