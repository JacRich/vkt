#ifndef RENDER_H
#define RENDER_H
#include "globaldef.h"
#include "mesh.h"
#include "region.h"
#include "player.h"
#include "light.h"

struct view_t
{
  mat4 view, proj;
};

void render_init();
void render_tick();
void render_terminate();

#endif

