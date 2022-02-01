#include "hud.h"
#include "render.h"

mesh_t* crosshair;
mesh_t* mesh_activeblock;

void hud_init()
{
  // Make spinny cube thing that shows selected block type
  mesh_activeblock  = render_addmesh();
  *mesh_activeblock = mesh_load_obj("gamedata/hud_cube.obj");

  mesh_activeblock->shader = sh_hud;
  mesh_activeblock->texture = tex_atlas;
  mesh_activeblock->rotation = 1.0f;
  mesh_activeblock->vertcount = 28 * 3;
  mesh_activeblock->pos = vec{2.6f, -1.3f, -3.0f};
  mesh_activeblock->scale *= 0.75f;
  mesh_activeblock->drawflags = DF_VIS; // No depth testing 
  
  // Make crosshair
  crosshair = render_addmesh();
  *crosshair = mesh_load_obj("gamedata/hud_crosshair.obj");
  crosshair->drawflags = DF_VIS; // No depth testing
  crosshair->shader    = sh_cross;
}

void hud_tick()
{
  // Make block's texture match the player's active block
  mesh_activeblock->customAttrib = player.active;
}

