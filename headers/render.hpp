#ifndef RENDER_H
#define RENDER_H
#include "globaldef.hpp"
#include "mesh.hpp"
#include "cursor.hpp"
#include "veng.hpp"

struct ubo_t
{
  uint handle;
  uint size;
};

struct view_t
{
  mat4 view, proj;
};

void update_ubos(player_t* player);
void render_init();
void render_tick();
void render_terminate();
void render_addmesh(mesh_t** mesh);

#endif

