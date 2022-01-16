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


void save_chunk(chunk_t* chunk);
int  load_chunk(chunk_t* chunk, ivec cord);
void chunk_fill(chunk_t* chunk);

#endif
