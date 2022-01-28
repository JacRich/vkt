#ifndef RENDER_H
#define RENDER_H
#include "globaldef.h"
#include "mesh.h"
#include "cursor.h"
#include "region.h"
#include "player.h"

struct ubo_t
{
  uint handle;
  uint size  ;
};

struct view_t
{
  mat4 view, proj;
};

void render_attach_cmeshes(region_t regions[REGION_COUNT]);
void render_addmesh(mesh_t** mesh);

void render_init();
void render_tick();
void render_terminate();

#endif

