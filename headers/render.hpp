#ifndef RENDER_H
#define RENDER_H
#include "globaldef.hpp"
#include "mesh.hpp"
#include "cursor.hpp"
#include "region.hpp"
#include "player.hpp"

#define CMESH_COUNT 64 * 27

struct ubo_t
{
  uint handle;
  uint size  ;
};

struct view_t
{
  mat4 view, proj;
};

void render_attach_cmeshes();
void render_addmesh(mesh_t** mesh);

void render_init();
void render_tick(player_t* player);
void render_terminate();

#endif

