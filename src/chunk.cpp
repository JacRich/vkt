#include "chunk.hpp"
#include "perlin.hpp"


void chunk_fill_perlin(chunk_t* chunk)
{   
  for(int i = 0; i < VOXELS_LENGTH; i++)
  {
    ivec index = index3d(i, 32);

    const double samplingScale = 38;
    double perlinSample = perlin_3d((chunk->pos.x + index.x + 500) / samplingScale, 
                                    (chunk->pos.y + index.y + 500) / samplingScale, 
                                    (chunk->pos.z + index.z + 500) / samplingScale);
    
    if(perlinSample > 0.55){
      chunk->voxels[index.x][index.y][index.z] = 0;
    }
    else {
      chunk->voxels[index.x][index.y][index.z] = V_STONE_GREY;
    }
  }
}

void chunk_fill(chunk_t* chunk, uchar value)
{   
  for(int x = 0; x < 32; x++)
  {
    for(int y = 0; y < 32; y++)
    {
      for(int z = 0; z < 32; z++)
      {             
        chunk->voxels[x][y][z] = value;            
      }
    }
  }
}
