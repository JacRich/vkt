#include "item.h"
#include "render.h"

void item_tick(void* owner)
{
  //printf("I exist!\n");
}

void item_init(item_t* item, vec pos)
{
  item->mesh  = render_addmesh();
  *item->mesh = mesh_load_obj("gamedata/test.obj");
  item->mesh->shader = sh_item;
  item->mesh->texture = tex_item;
  item->mesh->pos = pos;

  item->thinker = thinker_add();
  item->thinker->owner = item;
  item->thinker->func_tick = item_tick;
}