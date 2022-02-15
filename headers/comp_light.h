#ifndef LIGHT_H
#define LIGHT_H

#include "globaldef.h"

struct light_t
{
  vec4 pos;
  vec4 color = {0.9f, 0.65f, 0.5f, 1.0};
};

void lights_tick();

#endif
