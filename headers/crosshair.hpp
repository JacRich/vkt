#ifndef CROSSHAIR_H
#define CROSSHAIR_H
#include "globaldef.hpp"
#include "mesh.hpp"

struct crosshair_t
{
  mesh_t* mesh;
};

void crosshair_init(crosshair_t*, float);

#endif
