#ifndef TORCH_H
#define TORCH_H
#include "globaldef.hpp"
#include "entity.hpp"

struct torch_t
{
  vec pos;
  vec color = vec{1.0f, 0.45f, 0.05f};

  // time the torch will last in seconds
  float lifetime = 5.0f;

  entity_t* entity;
};

void torch_init(torch_t*);

#endif
