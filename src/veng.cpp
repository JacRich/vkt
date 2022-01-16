#include "globaldef.hpp"
#include "veng.hpp"
#include "cmesh.hpp"

#include <stdlib.h>
#include <sys/unistd.h>
#include <pthread.h>

extern player_t    player;
extern GLFWwindow* window;

cmesh_t cmeshes[CHUNKS_LENGTH];

region_t region;


region_t* find_region(vec pos)
{
    if(!inRange(pos, region.pos, region.pos + ivec{(8*32),(8*32),(8*32)})){
        return NULL;
    }
    return &region;
}

vhit veng_sample_pos(vec pos)
{
    vhit hit;
    
    // Find reigon to search
    region_t* region_ptr = find_region(pos);
    if(region_ptr == NULL){
        hit.state = HIT_NULL;
        return hit;
    }

    ivec voxelIndex = ivec{int(pos.x), int(pos.y), int(pos.z)};
    voxelIndex = ivec{voxelIndex.x % 32, voxelIndex.y % 32, voxelIndex.z % 32};

    ivec chunkIndex = ivec{int(pos.x), int(pos.y), int(pos.z)};
    chunkIndex = ivec{chunkIndex.x / 32, chunkIndex.y / 32, chunkIndex.z / 32};

    int      voxelValue =  region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[voxelIndex.x][voxelIndex.y][voxelIndex.z];
    uchar*   voxel_ptr  = &region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[voxelIndex.x][voxelIndex.y][voxelIndex.z];
    chunk_t* chunk_ptr  = &region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
    if (voxelValue != 0)
    {              
        hit.state = HIT_TRUE;
        hit.voxel = voxel_ptr;
        hit.chunk = chunk_ptr;
        hit.pos   = pos;      
        return hit;
    }
    else
    {                   
        hit.state = HIT_FALSE;       
        hit.voxel = voxel_ptr;
        hit.chunk = chunk_ptr;
        hit.pos   = pos;             
        return hit;
    }
}

vhit veng_raycast(int range, vec rayStart, vec rayDir)
{      
    vhit lastTest;

    int stepCount = 0;
    vec  rayUnitStepSize = {abs(1.0f / rayDir.x), abs(1.0f / rayDir.y), abs(1.0f / rayDir.z)};
    ivec step       ;
    vec  rayLength1D;
    ivec voxelCheck = {int(rayStart.x), int(rayStart.y), int(rayStart.z)};

    // Establish Starting Conditions
    if (rayDir.x < 0)
    {
        step.x = -1;
        rayLength1D.x = (rayStart.x - (float)(voxelCheck.x)) * rayUnitStepSize.x;
    }
    else
    {
        step.x = 1;
        rayLength1D.x = ((float)(voxelCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
    }
    if (rayDir.y < 0)
    {
        step.y = -1;
        rayLength1D.y = (rayStart.y - (float)(voxelCheck.y)) * rayUnitStepSize.y;
    }
    else
    {
        step.y = 1;
        rayLength1D.y = ((float)(voxelCheck.y + 1) - rayStart.y) * rayUnitStepSize.y;
    }
    if (rayDir.z < 0)
    {
        step.z = -1;
        rayLength1D.z = (rayStart.z - (float)(voxelCheck.z)) * rayUnitStepSize.z;
    }
    else
    {
        step.z = 1;
        rayLength1D.z = ((float)(voxelCheck.z + 1) - rayStart.z) * rayUnitStepSize.z;
    }

    bool  voxelFound  = false;
    float maxDistance = float(range);
    float distance    = 0.0f;

    while (!voxelFound && distance < maxDistance)
    {
        // Walk along shortest path
        if (rayLength1D.x < rayLength1D.y && rayLength1D.x < rayLength1D.z)
        {
            voxelCheck.x += step.x;
            distance = rayLength1D.x;
            rayLength1D.x += rayUnitStepSize.x;
        }
        else if (rayLength1D.y < rayLength1D.x && rayLength1D.y < rayLength1D.z)
        {
            voxelCheck.y += step.y;
            distance = rayLength1D.y;
            rayLength1D.y += rayUnitStepSize.y;
        }
        else
        {
            voxelCheck.z += step.z;
            distance = rayLength1D.z;
            rayLength1D.z += rayUnitStepSize.z;
        }
        
        vhit test = veng_sample_pos(vec{voxelCheck.x, voxelCheck.y, voxelCheck.z});
        
        if(test.state == HIT_TRUE)
        {
            if(stepCount < 1) // If we hit at first step, set last hit data equal to current
            {
                lastTest.voxel = test.voxel;
                lastTest.pos   = test.pos;
            }
            voxelFound = true;
            test.voxelLast = lastTest.voxel;
            test.posLast   = lastTest.pos;
              
            return test;             
        }
        else
        {
            lastTest = test;
        } 
        stepCount++;
    }
        
    return lastTest;
}

void veng_change_withcursor(cursor_t* cursor, uchar newValue, int collMode)
{
    int cubedRange = cursor->size * cursor->size * cursor->size;
    ivec truncPos = vecToIvec(cursor->mesh->pos);

    for(int i = 0; i < cubedRange; i++)
    {
        ivec cord     = index3d(i, cursor->size) + truncPos;     
        vec  voxelPos = ivecToVec(cord);
        vhit hit      = veng_sample_pos(voxelPos);
        
        if(hit.state == collMode) // Fill or Replace
        {
            *hit.voxel = newValue;
            hit.chunk->update = true;
        }
    }
}

void veng_change_voxel(int pickmode, vhit voxel, uchar value)
{   
    switch (pickmode) // Change hit voxel or last hit voxel
    {
        case(PICK_HIT):
        *voxel.voxel = value;
        voxel.chunk->update = true;
        break;

        case(PICK_NORMAL):
        *voxel.voxelLast = value;
        voxel.chunk->update = true;
        break;
    }
}

void veng_tick()
{
    
}

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
    sprintf(buffer, "%i",  cord.z);
    strcat(path, buffer);
}

void save_region(region_t* region)
{
    char path [20] = "savedata/r_";
    make_path(path, region->cord);

    FILE* file;
    file = fopen(path, "wb");
    
    for(int i = 0; i < 512; i++)
    {
        ivec chunkIndex = index3d(i, 8);
        int writeTotalVoxels = 0;
        int lastWriteIndex   = 0;
        int writeCount       = 0;
        sector_t buffer;

        for(int i = 0; i < VOXELS_LENGTH; i++)
        {       
            ivec voxelIndex = index3d(i, 32);
            int voxelValue = region->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[voxelIndex.x][voxelIndex.y][voxelIndex.z];


            if(i == VOXELS_LENGTH - 1) // Are we at the end?
            {
                buffer.size = i - lastWriteIndex;
                buffer.value = voxelValue;
                fwrite((const void*)&buffer, sizeof(buffer), 1, file);
                lastWriteIndex = i;
                writeCount++;
                writeTotalVoxels += buffer.size;
                continue;
            }
            if(writeCount == 0)
            {
                buffer.size = i + 1;
            }
            else
            {
                buffer.size = i - lastWriteIndex;
            }

            if((i - lastWriteIndex) > 250) // Cap size of sector @ uchar limit. Why 250? Not sure Im too dumb
            {
                buffer.value = voxelValue;
                fwrite((const void*)&buffer, sizeof(buffer), 1, file);
                lastWriteIndex = i;
                writeCount++;
                writeTotalVoxels += buffer.size;
                continue;
            }
            ivec nextVoxel = index3d(i + 1, 32);
            if(voxelValue != region->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[nextVoxel.x][nextVoxel.y][nextVoxel.z])
            {   
                buffer.value = voxelValue;
                fwrite((const void*)&buffer, sizeof(buffer), 1, file);
                lastWriteIndex = i;
                writeCount++;
                writeTotalVoxels += buffer.size;
            }
        }
        assert(writeTotalVoxels == VOXELS_LENGTH);
    }
    
    
    fclose(file);
}

void load_region(region_t* region)
{
    char path [20] = "savedata/r_";
    make_path(path, region->cord);

    if(access(path, F_OK) != 0) // Does file exist
    {      
        return;
    }
    FILE* file = fopen(path, "rb");

    int readTotal = 0;
    int voxel     = 0;
    sector_t buffer;
    
    for(int i = 0; i < 512; i++)
    {
        ivec chunkindex = index3d(i, 8);
        while(voxel < VOXELS_LENGTH)
        {
            fread(&buffer, sizeof(buffer), 1, file);
            readTotal += buffer.size;

            for(int j = 0; j < buffer.size; j++)
            {
                ivec index = index3d(voxel, 32);
                region->chunks[chunkindex.x][chunkindex.y][chunkindex.z].voxels[index.x][index.y][index.z] = buffer.value;
                voxel++;
            }
        }
        voxel = 0;
    }    
    //assert(readTotal == VOXELS_LENGTH);

    fclose(file);
}

void veng_init()
{
  load_region(&region);
  for(int i = 0; i < 512; i++)
  {
    ivec fuck = index3d(i, 8);
    region.chunks[fuck.x][fuck.y][fuck.z].pos = vec{fuck.x * 32, fuck.y * 32, fuck.z * 32};

    cmesh_init(&cmeshes[i]);
    cmesh_build(&cmeshes[i], &region.chunks[fuck.x][fuck.y][fuck.z]); 
  }
}

void* veng_terminate(void* args)
{
    double time = glfwGetTime();
    save_region(&region);
    printf("Time: %.6f\n", glfwGetTime() - time);
    return NULL;
}
