#ifndef CROSSHAIR_H
#define CROSSHAIR_H
#include "globaldef.h"
#include "mesh.h"

struct crosshair_t
{
  mesh_t* mesh;
};

void crosshair_init(crosshair_t*, float);

#endif
