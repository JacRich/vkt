#ifndef PHYS_H
#define PHYS_H
#include "globaldef.h"


struct coll_sphere_t
{
  vec pos = {0,0,0};
  float extent = 10.0f;
};

void phys_init();
void phys_tick();

coll_sphere_t* phys_add_collider();


#endif