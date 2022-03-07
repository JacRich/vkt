#ifndef IVEC_H
#define IVEC_H

#include "globaldef.h"

struct ivec
{
  int x, y, z;

  int operator==(ivec left)
  {
    if (x == left.x && y == left.y && z == left.z)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  int operator!=(ivec left)
  {
    if (x != left.x || y != left.y || z != left.z)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  ivec operator-(ivec left)
  {
    return {x - left.x, y - left.y, z - left.z};
  }

  ivec operator-(vec left)
  {
    return {x - int(left.x), y - int(left.y), z - int(left.z)};
  }

  ivec operator+(ivec left)
  {
    return {x + left.x, y + left.y, z + left.z};
  }

  ivec operator+(vec left)
  {
    return ivec{x + int(left.x), y + int(left.y), z + int(left.z)};
  }

  ivec operator/=(int left)
  {
    return ivec{x / left, y / left, z / left};
  }

  ivec operator*=(int left)
  {
    return ivec{x * left, y * left, z * left};
  }

  ivec operator%=(int left)
  {
    return ivec{x % left, y % left, z % left};
  }
};

struct ivec2
{
  int x, y;
};

vec4 vec_to_vec4(vec ass);

vec vec4_to_vec(vec4 ass);

ivec region_cord_at(vec worldpos);

vec ivec_to_vec(ivec left);

ivec vec_to_ivec(vec left);

int taxi_dist(ivec left, ivec right);

ivec index3d(int index1d, int cuberoot);
ivec2 index2d(int index1d, int squareroot);

int index1d(ivec cord, int maxX, int maxY);
int index1d(ivec2 cord, int maxX);

void moveCords(ivec *cords, ivec moveDir);

bool AABB(vec a, vec rangeMin, vec rangeMax);

void print_ivec(ivec n);

void print_vec(vec n);

#endif