#ifndef IVEC_H
#define IVEC_H
#include "globaldef.hpp"
using vec = glm::vec3;

struct ivec
{
    int x, y, z;

    int operator ==(ivec left)
    {   
        if(x == left.x && y == left.y && z == left.z)
        {
            return 1;
        }
        else 
        {
            return 0;
        }
    }

    int operator !=(ivec left)
    {   
        if(x != left.x || y != left.y || z != left.z)
        {
            return 1;
        }
        else 
        {
            return 0;
        }
    }
    
    ivec operator -(ivec left)
    {
        return {x - left.x, y - left.y, z - left.z};
    }

    ivec operator -(vec left)
    {
        return {x - int(left.x), y - int(left.y), z - int(left.z)};
    }

    ivec operator +(ivec left)
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

int ivecDist(ivec left, ivec right);

vec  ivecToVec(ivec left);
ivec vecToIvec(vec left);

ivec index3d(int index1d, int cuberoot);
int  index1d(ivec cord, int maxX, int maxY);

void moveCords(ivec* cords, ivec moveDir);

bool inRange(ivec a, ivec rangeMin, ivec rangeMax);
bool inRange(vec  a, ivec rangeMin, ivec rangeMax);

void printIvec(ivec n);
void printvec(vec n);

#endif