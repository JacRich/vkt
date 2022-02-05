#include "hud.h"
#include "render.h"
#include "ecs.h"
#include "player.h"
#include "veng.h"
#include "config.h"

entity_t block_display;
mesh_t* block_display_mesh;

entity_t crosshair;
mesh_t* mesh_cross;

entity_t cursor;
mesh_t* cursor_mesh;
transform_t* cursor_transform;


void hud_init() {
  // Make display block thingy
  block_display = entity_add(C_MESH | C_TRANSFORM);
  block_display_mesh = &components.meshes[block_display];
  *block_display_mesh = mesh_load_obj("gamedata/hud_cube.obj", sh_hud, &tex_atlas, DF_NO_DEPTH);
  components.transforms[block_display].pos = vec{2.6f, -1.3f, -3.0f};
  components.transforms[block_display].scale *= 0.7f;

  // Make crosshair
  crosshair = entity_add(C_MESH | C_TRANSFORM);
  mesh_cross = &components.meshes[crosshair];
  *mesh_cross = mesh_load_obj("gamedata/hud_crosshair.obj", sh_cross, NULL, DF_NO_DEPTH);

  // Make cursor
  cursor = entity_add(C_MESH | C_TRANSFORM);
  cursor_mesh = &components.meshes[cursor];
  *cursor_mesh = mesh_load_obj("gamedata/hud_cursor.obj", sh_cursor, NULL, 0);
  cursor_mesh->polymode = GL_LINE;
  cursor_transform = &components.transforms[cursor];
  cursor_transform->pos = player.pos;
}

void hud_tick() 
{
  if(config.hideGUI){
    cursor_mesh->drawflags =        DF_NO_DRAW;
    block_display_mesh->drawflags = DF_NO_DRAW;
    mesh_cross->drawflags =         DF_NO_DRAW;
    return;
  }
  cursor_mesh->drawflags        = DF_DEFAULT;
  block_display_mesh->drawflags = DF_HUD;
  mesh_cross->drawflags         = DF_HUD;

  // Make block's texture match the player's active block
  block_display_mesh->customAttrib = player.active;

  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if (hit.state != HIT_TRUE) {
    cursor_mesh->drawflags = DF_NO_DRAW;
    return;
  }
  
  cursor_transform->pos = hit.pos;
}

