#include "region.hpp"
#include <stdlib.h>
#include <sys/unistd.h>
#include <pthread.h>

struct sector_t
{
  uchar size = 0;
  uchar value = 0;
};

static void make_path(char path[], ivec cord)
{
  char buffer[10];

  // X
  sprintf(buffer, "%i_", cord.x);
  strcat(path, buffer);

  // Y
  sprintf(buffer, "%i_", cord.y);
  strcat(path, buffer);

  // Z
  sprintf(buffer, "%i", cord.z);
  strcat(path, buffer);
}

void region_save(region_t *region)
{
  char path[25] = "gamedata/savedata/r_";
  make_path(path, region->cord);

  FILE *file;
  file = fopen(path, "wb");

  // Loop for chunks in region
  for (int i = 0; i < REGION_CHUNK_COUNT; i++)
  {
    ivec chunkIndex = index3d(i, REGION_CROOT);
    int writeTotalVoxels = 0;
    int lastWriteIndex = 0;
    int writeCount = 0;
    sector_t buffer;

    for (int i = 0; i < VOXELS_LENGTH; i++)
    {
      ivec voxelIndex = index3d(i, CHUNK_CROOT);
      int voxelValue = region->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[voxelIndex.x][voxelIndex.y][voxelIndex.z];

      // Are we at the end?
      if (i == VOXELS_LENGTH - 1){
        buffer.size = i - lastWriteIndex;
        buffer.value = voxelValue;
        fwrite((const void *)&buffer, sizeof(buffer), 1, file);

        lastWriteIndex = i;
        writeCount++;
        writeTotalVoxels += buffer.size;
        break;
      }

      // Have we written a sector yet
      if (writeCount == 0){
        buffer.size = i + 1;
      }
      else{
        buffer.size = i - lastWriteIndex;
      }
      
      // Cap size of sector @ uchar limit
      if (buffer.size > 254){
        buffer.value = voxelValue;
        fwrite((const void *)&buffer, sizeof(buffer), 1, file);
        lastWriteIndex = i;
        writeCount++;
        writeTotalVoxels += buffer.size;
        continue;
      }

      ivec nextVoxel = index3d(i + 1, CHUNK_CROOT);
      if (voxelValue != region->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[nextVoxel.x][nextVoxel.y][nextVoxel.z])
      {
        buffer.value = voxelValue;
        fwrite((const void *)&buffer, sizeof(buffer), 1, file);
        lastWriteIndex = i;
        writeCount++;
        writeTotalVoxels += buffer.size;
      }
    }
    assert(writeTotalVoxels == VOXELS_LENGTH);
  }

  fclose(file);
}

void region_load(region_t *region)
{
  char path[25] = "gamedata/savedata/r_";
  make_path(path, region->cord);

  if (access(path, F_OK) != 0) // Does file exist
  {
    region_fill_perlin(region);
    return;
  }


  FILE *file = fopen(path, "rb");

  int readTotal = 0;
  int voxel = 0;
  sector_t buffer;

  // Loop for chunks in region
  for (int i = 0; i < REGION_CHUNK_COUNT; i++)
  {
    // Loop until we have uncompressed to VOXELS_LENGTH
    ivec chunkindex = index3d(i, REGION_CROOT);
    while (voxel < VOXELS_LENGTH)
    {
      // Read one sector into buffer
      fread(&buffer, sizeof(buffer), 1, file);
      readTotal += buffer.size;

      // Loop for size of buffer, adding the sector buffer's value into the chunk
      for (int j = 0; j < buffer.size; j++)
      {
        ivec index = index3d(voxel, CHUNK_CROOT);
        region->chunks[chunkindex.x][chunkindex.y][chunkindex.z].voxels[index.x][index.y][index.z] = buffer.value;
        voxel++;
      }
    }
    voxel = 0;
    region->chunks[chunkindex.x][chunkindex.y][chunkindex.z].update = true;
  }
  //assert(readTotal == VOXELS_LENGTH);

  fclose(file);
}

void region_fill_value(region_t *region, uchar value)
{
  for (int i = 0; i < REGION_CHUNK_COUNT; i++)
  {
    ivec position = index3d(i, REGION_CROOT);
    chunk_t* chunk = &region->chunks[position.x][position.y][position.z];

    chunk_fill(chunk, value);
    chunk->update = true;
  }
}

void region_fill_perlin(region_t *region)
{
  for (int i = 0; i < REGION_CHUNK_COUNT; i++)
  {
    ivec position = index3d(i, REGION_CROOT);
    chunk_t* chunk = &region->chunks[position.x][position.y][position.z];

    chunk_fill_perlin(chunk);
    chunk->update = true;
  }
}

void region_set_pos(region_t *region, ivec cord)
{
  region->pos = vec{cord.x * REGION_WIDTH, cord.y * REGION_WIDTH, cord.z * REGION_WIDTH};
  region->cord = cord;

  // Set World pos of all chunks
  for (int i = 0; i < REGION_CHUNK_COUNT; i++)
  {
    ivec position = index3d(i, REGION_CROOT);
    chunk_t* chunk = &region->chunks[position.x][position.y][position.z]; 

    chunk->pos = vec{position.x * CHUNK_CROOT, position.y * CHUNK_CROOT, position.z * CHUNK_CROOT} + region->pos;
    //chunk_fill_perlin(chunk);
    chunk->update = true;
  }
}