#include "chunk.hpp"
#include "cmesh.hpp"
#include "perlin.hpp"
#include "render.hpp"
#include <stdlib.h>
#include <sys/unistd.h>


void chunk_fill(chunk_t* chunk)
{   
    for(int x = 0; x < 32; x++)
    {
        for(int y = 0; y < 32; y++)
        {
            for(int z = 0; z < 32; z++)
            {
                const double samplingScale = 38;
                double perlinSample = perlin_3d((chunk->pos.x + x + 500) / samplingScale, 
                                                (chunk->pos.y + y + 500) / samplingScale, 
                                                (chunk->pos.z + z + 500) / samplingScale);
                
                if(perlinSample > 0.55)
                {
                    chunk->voxels[x][y][z] = 0;
                    continue;
                }
                if(perlinSample < 0.55)
                {
                    chunk->voxels[x][y][z] = V_ANDE;
                }
            }
        }

    }
}
