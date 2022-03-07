#include "hud.h"
#include "window.h"
#include "ecs.h"
#include "player.h"
#include "world.h"
#include "config.h"


mesh_t* torch_mesh;

entity_t block_display;
mesh_t*  block_display_mesh;

entity_t crosshair;
mesh_t*  mesh_cross;

entity_t     cursor;
mesh_t*      cursor_mesh;
transform_t* cursor_transform;

entity_t cursor_region;
mesh_t*  cursor_region_mesh;
transform_t* cursor_region_transform;

entity_t cursor_range;
mesh_t* cursor_range_mesh;
transform_t* cursor_range_transform;

void hud_init()
{
  // Make display block thingy
  block_display = entity_add(C_MESH | C_TRANSFORM);
  block_display_mesh = &components.meshes[block_display];
  *block_display_mesh = mesh_load("gamedata/models/hud_cube.obj", sh_hud, &tex_atlas, DF_NO_DEPTH);

  // Make crosshair
  crosshair = entity_add(C_MESH | C_TRANSFORM);
  mesh_cross = &components.meshes[crosshair];
  *mesh_cross = mesh_load("gamedata/models/hud_crosshair.obj", sh_cross, NULL, DF_NO_DEPTH);

  // Make cursor
  cursor = entity_add(C_MESH | C_TRANSFORM);
  cursor_mesh = &components.meshes[cursor];
  *cursor_mesh = mesh_load("gamedata/models/hud_cursor.obj", sh_cursor, NULL, 0);
  cursor_mesh->polymode = GL_LINE;
  cursor_transform = &components.transforms[cursor];
  cursor_transform->pos = player.pos;

  // Make region cursor
  cursor_region = entity_add(C_MESH | C_TRANSFORM);
  cursor_region_mesh = &components.meshes[cursor_region];
  *cursor_region_mesh = mesh_load("gamedata/models/hud_cursor.obj", sh_cursor, NULL, DF_NO_CULL);
  cursor_region_mesh->polymode = GL_LINE;
  cursor_region_transform = &components.transforms[cursor_region];
  cursor_region_transform->pos = player.pos;
  cursor_region_transform->scale = vec{REGION_WIDTH, REGION_WIDTH, REGION_WIDTH};

  // Make range cursor
  cursor_range = entity_add(C_MESH | C_TRANSFORM);
  cursor_range_mesh = &components.meshes[cursor_range];
  *cursor_range_mesh = mesh_load("gamedata/models/hud_cursor.obj", sh_cursor, NULL, DF_NO_CULL);
  cursor_range_mesh->polymode = GL_LINE;
  cursor_range_transform = &components.transforms[cursor_range];
  
}

void hud_tick()
{
  if (config.hideGUI)
  {
    cursor_mesh->drawflags = DF_NO_DRAW;
    block_display_mesh->drawflags = DF_NO_DRAW;
    mesh_cross->drawflags = DF_NO_DRAW;
    return;
  }

  if(player.selected_region != NULL){
    cursor_region_transform->pos = player.selected_region->pos;
  }

  cursor_mesh->drawflags = DF_DEFAULT;
  cursor_range_mesh->drawflags = DF_DEFAULT;
  block_display_mesh->drawflags = DF_HUD;
  mesh_cross->drawflags = DF_HUD;

  // Make block's texture match the player's active block
  block_display_mesh->customAttrib = player.active;

  vhit hit = world_raycast(player.reach, player.pos, player.front);
  if (hit.state != HIT_TRUE)
  {
    cursor_mesh->drawflags = DF_NO_DRAW;
    cursor_range_mesh->drawflags = DF_NO_DRAW;
    return;
  }
  if(!player.useRange){
    cursor_range_mesh->drawflags = DF_NO_DRAW;
  }

  cursor_transform->pos = hit.pos;
  cursor_range_transform->pos = hit.pos - vec{float(player.range / 2)};
  cursor_range_transform->scale = vec{player.range, player.range, player.range};
}
