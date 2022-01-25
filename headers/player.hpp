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
  
  vec front_walk = {0.0f, 0.0f, 0.0f};
  vec right_walk = {0.0f, 0.0f, 0.0f};

  uchar active = 1;

  int reach = 20;
  
  int editor = true;
  int inputMode = IM_BLOCKS;

  float pitch = 0.0f, yaw = 0.0f;

  bool grounded = false;

  vec lightColor = vec{1.0f, 0.45f, 0.05f};
};

extern player_t player;

void player_init();
void player_tick();
void player_terminate();

#endif
