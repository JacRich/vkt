#include "globaldef.h"
#include "veng.h"
#include "cmesh.h"
#include <thread>

region_t regions[REGION_COUNT];
ivec     r_cords[REGION_COUNT];
ivec r_cord_now, r_cord_last;

static std::thread thread;


chunk_t* veng_find_chunk(vec worldpos)
{
  region_t *region_ptr = veng_find_region(worldpos);
  if (region_ptr == NULL){
    return NULL;
  }
  vec relativepos = worldpos - region_ptr->pos;
  ivec chunkIndex = vec_to_ivec(relativepos);
  chunkIndex = ivec{chunkIndex.x / CHUNK_CROOT, chunkIndex.y / CHUNK_CROOT, chunkIndex.z / CHUNK_CROOT};

  return &region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
}

region_t* veng_find_region(vec worldpos)
{
  for (int i = 0; i < REGION_COUNT; i++)
  {
    if (AABB(worldpos, regions[i].pos, regions[i].pos + vec{REGION_WIDTH, REGION_WIDTH, REGION_WIDTH}))
    {
      return &regions[i];
    }
  }
  return NULL;
}

vhit veng_find_voxel(vec worldpos)
{
  vhit hit;
  vec pos_regiontest = worldpos + vec{0.1f, 0.1f, 0.1f}; // Offset the pos we test for bc reasons?

  // Find reigon to search
  region_t *region_ptr = veng_find_region(pos_regiontest);
  if (region_ptr == NULL){
    hit.state = HIT_NULL;
    return hit;
  }
  vec relativepos = worldpos - region_ptr->pos;

  ivec voxelIndex = ivec{int(relativepos.x), int(relativepos.y), int(relativepos.z)};
  voxelIndex = ivec{voxelIndex.x % CHUNK_CROOT, voxelIndex.y % CHUNK_CROOT, voxelIndex.z % CHUNK_CROOT};

  ivec chunkIndex = ivec{int(relativepos.x), int(relativepos.y), int(relativepos.z)};
  chunkIndex = ivec{chunkIndex.x / CHUNK_CROOT, chunkIndex.y / CHUNK_CROOT, chunkIndex.z / CHUNK_CROOT};

  int voxelValue = region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[voxelIndex.x][voxelIndex.y][voxelIndex.z];
  uchar *voxel_ptr = &region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].voxels[voxelIndex.x][voxelIndex.y][voxelIndex.z];
  chunk_t *chunk_ptr = &region_ptr->chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
  if (voxelValue != 0)
  {
    hit.state = HIT_TRUE;
    hit.voxel = voxel_ptr;
    hit.chunk = chunk_ptr;
    hit.pos   = worldpos; 
    return hit;
  }
  else
  {
    hit.state = HIT_FALSE;
    hit.voxel = voxel_ptr;
    hit.chunk = chunk_ptr;
    hit.pos = worldpos;
    return hit;
  }
}

vhit veng_raycast(int range, vec rayStart, vec rayDir)
{
  vhit lastTest;

  int stepCount = 0;
  vec rayUnitStepSize = {abs(1.0f / rayDir.x), abs(1.0f / rayDir.y), abs(1.0f / rayDir.z)};
  ivec step;
  vec rayLength1D;
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

  bool voxelFound = false;
  float maxDistance = float(range);
  float distance = 0.0f;

  while (!voxelFound && distance < maxDistance)
  {
    // Step along shortest path
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

    vhit test = veng_find_voxel(vec{voxelCheck.x, voxelCheck.y, voxelCheck.z});

    if (test.state == HIT_TRUE)
    {
      if (stepCount < 1) // If we hit at first step, set last hit data equal to current
      {
        lastTest.voxel = test.voxel;
        lastTest.pos = test.pos;
        lastTest.chunk = test.chunk;
      }
      voxelFound = true;
      test.voxelLast = lastTest.voxel;
      test.posLast = lastTest.pos;
      test.chunkLast = lastTest.chunk;

      test.distance = glm::length(test.pos - rayStart);

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

void veng_change_withcursor(cursor_t* cursor, uchar newValue, int filter)
{
  int cubedRange = cursor->size * cursor->size * cursor->size;
  ivec truncPos = vec_to_ivec(cursor->mesh->pos);

  for (int i = 0; i < cubedRange; i++)
  {
    ivec cord = index3d(i, cursor->size) + truncPos;
    vec voxelPos = ivec_to_vec(cord);
    vhit hit = veng_find_voxel(voxelPos);
    // Fill or Replace
    if (hit.state == filter) 
    {
      *hit.voxel = newValue;
      hit.chunk->update = true;
    }
  }
}

void veng_change_voxel(vhit voxel, int pickmode, uchar value)
{
  // PICK_HIT means to change hit voxel, otherwise change voxelLast
  if(pickmode == PICK_HIT){
    *voxel.voxel = value;
    voxel.chunk->update = true;
  }
  else if (voxel.voxelLast != NULL){
    *voxel.voxelLast = value;
    voxel.chunkLast->update = true;
  }
}


static bool is_region_inrange(region_t* region)
{
  for(int i = 0; i < REGION_COUNT; i++){
    if(region->cord == r_cords[i]){
      return true;
    }
  }
  return false;
}

static bool is_cord_filled(ivec cord)
{
  for(int i = 0; i < REGION_COUNT; i++){
    if(regions[i].cord == cord){
      return true;
    }
  }
  return false;
}

static void update_region()
{
  for(int i = 0; i < REGION_COUNT; i++)
  {
    if(!is_region_inrange(&regions[i])){
      for(int j = 0; j < REGION_COUNT; j++)
      {
        if(!is_cord_filled(r_cords[j])){
          region_save(&regions[i]);
          regions[i] = region_load(r_cords[j]);
          return;
        }
      }
    }
  }
}

static void tick_update_regions()
{
  while(!glfwWindowShouldClose(window)){
    update_region();
  }
}


void veng_init()
{
  r_cord_now  = region_cord_at(player.pos + vec{32.0f, 32.0f, 32.0f});
  r_cord_last = r_cord_now;

  // Build a 3x3x3 area of regions with the player in the center
  for(int i = 0; i < REGION_COUNT; i++){
    ivec r_cord = index3d(i, REGION_COUNT_CROOT);
    ivec transformed_rcord = region_cord_at(player.pos) + r_cord - ivec{REGION_COUNT_CROOT / 2,REGION_COUNT_CROOT / 2,REGION_COUNT_CROOT / 2};

    regions[i] = region_load(transformed_rcord);
    r_cords[i] = transformed_rcord;
  }

  render_attach_cmeshes(regions);

  thread = std::thread(tick_update_regions);
}

void transform_cords(ivec dir)
{
  for(int i = 0; i < REGION_COUNT; i++){
    r_cords[i] = r_cords[i] + dir; 
  }
}

void veng_tick()
{
  r_cord_now = region_cord_at(player.pos + vec{32.0f, 32.0f, 32.0f});
  if(r_cord_now != r_cord_last){
    transform_cords(r_cord_now - r_cord_last);
  }
  r_cord_last = r_cord_now;
}

void veng_terminate()
{
  for (int i = 0; i < REGION_COUNT; i++){
    region_save(&regions[i]);
  }

  thread.join();
  thread.~thread();
}
