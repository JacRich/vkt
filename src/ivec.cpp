#include <stdio.h>
#include "ivec.h"


vec vec4_to_vec(vec4 ass)
{
  return{ass.x, ass.y, ass.z};
}

vec4 vec_to_vec4(vec ass)
{
  return{ass.x, ass.y, ass.z, 1.0f};
}

ivec region_cord_at(vec worldpos)
{
  ivec result;
  result.x = worldpos.x > 0? int(worldpos.x / REGION_WIDTH) : int(worldpos.x / REGION_WIDTH) + -1;
  result.y = worldpos.y > 0? int(worldpos.y / REGION_WIDTH) : int(worldpos.y / REGION_WIDTH) + -1;
  result.z = worldpos.z > 0? int(worldpos.z / REGION_WIDTH) : int(worldpos.z / REGION_WIDTH) + -1;

  return result;
}

int taxi_dist(ivec left, ivec right)
{
  return abs(left.x - right.x) + abs(left.y - right.y) + abs(left.z - right.z);
}

vec ivec_to_vec(ivec left)
{
  return vec{float(left.x), float(left.y), float(left.z)};
}

ivec vec_to_ivec(vec left)
{
  return ivec{int(left.x), int(left.y), int(left.z)};
}

ivec index3d(int index1d, int cuberoot)
{
  int z    = index1d / (cuberoot * cuberoot);
  index1d -= z * cuberoot * cuberoot;
  int y    = index1d / cuberoot;
  int x    = index1d % cuberoot;

  return {x, y, z};
}

int index1d(ivec cord, int maxX, int maxY)
{
  return cord.x + maxX * (cord.y + (maxY * cord.z));
}

bool AABB(vec a, vec rangeMin, vec rangeMax)
{
  if(a.x <= rangeMin.x || a.y <= rangeMin.y || a.z <= rangeMin.z){
    return false;
  } 
  if(a.x >= rangeMax.x || a.y >= rangeMax.y || a.z >= rangeMax.z){
    return false;
  }  
  return true;
}

void print_ivec(ivec n)
{
  printf("%i,%i,%i \n", n.x, n.y, n.z);
}

void print_vec(vec n)
{
  printf("%.3f,%.3f,%.3f \n", n.x, n.y, n.z);
}
