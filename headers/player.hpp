#ifndef PLAYER_H
#define PLAYER_H
#include "globaldef.hpp"
#include "ivec.hpp"
#include "entity.hpp"

enum INPUT_MODES
{
  IM_BLOCKS,
  IM_BLOCKS_RANGE_FILL,
  IM_BLOCKS_RANGE_REPLACE
};

enum MOVEMENT_MODES
{
  MM_WALK,
  MM_FLY
};

struct player_t
{
  vec pos = {16.0f,16.0f,16.0f};
  vec vel;

  vec front = {0.0f, 0.0f,-1.0f};
  vec up    = {0.0f, 1.0f, 0.0f};
  vec right = {1.0f, 0.0f, 0.0f};
  
  vec frontMov = {0.0f, 0.0f, 0.0f};
  vec rightMov = {0.0f, 0.0f, 0.0f};

  uchar active = 1;

  int reach = 10;
  
  int moveMode  = MM_FLY   ;
  int inputMode = IM_BLOCKS;

  float accel    = 400.0f;
  float friction = 12.0f ;
  float sens     = 0.10f ;

  float pitch = 0.0f, yaw = 0.0f;

  bool grounded = false;

  entity_t* entity;
};

void player_init(player_t*);
void player_terminate(player_t*);

#endif
