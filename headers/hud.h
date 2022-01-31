#ifndef HUD_H
#define HUD_H
#include "globaldef.h"
#include "mesh.h"
#include "thinker.h"

struct hud_t
{
  mesh_t* mesh;
};

void hud_init(hud_t* hud);


#endif
