#ifndef REGION_H
#define REGION_H
#include "globaldef.hpp"
#include "chunk.hpp"
#include "ivec.hpp"

struct region_t
{
    ivec pos  = {0,0,0};
    ivec cord = {0,0,0};
    chunk_t chunks[8][8][8];

    bool replace = false;
    //bool lock = fal
};

#endif