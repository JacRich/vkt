#ifndef PLAYER_H
#define PLAYER_H

#include "globaldef.h"
#include "ivec.h"

struct player_t
{
  vec pos = {200.0f, 200.0f, 200.0f};
  vec vel = {0.0f, 0.0f, 0.0f};

  vec front = {0.0f, 0.0f, -1.0f};
  vec up = {0.0f, 1.0f, 0.0f};
  vec right = {1.0f, 0.0f, 0.0f};

  vec front_walk = {0.0f, 0.0f, 0.0f};
  vec right_walk = {0.0f, 0.0f, 0.0f};

  uint8 active = 6;
  int reach = 20;

  float pitch = 0.0f, yaw = 0.0f;

  bool flyingColl = false;
  bool flying = true;
  bool grounded = false;
};

extern player_t player;

void player_init();

void player_tick();

void player_terminate();

#endif
