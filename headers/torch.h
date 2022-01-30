#ifndef TORCH_H
#define TORCH_H
#include "globaldef.h"
#include "thinker.h"
#include "light.h"


struct torch_t
{
  light_t*   light;
  thinker_t* thinker;

  float fuel     = 120.0f;
  float lifetime = 120.0f;
};

void torch_init(torch_t* torch);


#endif