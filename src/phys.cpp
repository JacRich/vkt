#include "phys.h"
#include "player.h"

coll_sphere_t coll_spheres[20]; 


int coll_sphere_count = 0;



void phys_init()
{

}

void phys_tick()
{
  // Test for player overlap
  for(int i = 0; i < coll_sphere_count; i++){
    float dist = glm::length(coll_spheres[i].pos - player.pos);
    if(dist < 5.0f){ 
      //printf("HIT: %.2f\n", dist);
    }
  }

}


coll_sphere_t* phys_add_collider()
{ 
  coll_sphere_count++;
  return &coll_spheres[coll_sphere_count - 1];
}