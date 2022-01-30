#ifndef LIGHT_H
#define LIGHT_H
#include "globaldef.h"

struct light_t
{
  vec4 pos;
  vec4 color = {1.0f, 0.5f, 0.0f, 0.0f};
};


#endif
