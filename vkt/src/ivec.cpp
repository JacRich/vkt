#include <stdio.h>
#include "ivec.hpp"

int ivecDist(ivec left, ivec right)
{
    return abs(left.x - right.x) + abs(left.y - right.y) + abs(left.z - right.z);
}

vec ivecToVec(ivec left)
{
    return vec{float(left.x), float(left.y), float(left.z)};
}

ivec vecToIvec(vec left)
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

void moveCords(ivec* cords, ivec moveDir)
{
    for (int i = 0; i < CHUNKS_LENGTH; i++)
    {
        cords[i].x -= moveDir.x; cords[i].y -= moveDir.y; cords[i].z -= moveDir.z;
    }
}

bool inRange(ivec a, ivec rangeMin, ivec rangeMax)
{
    return a.x >= rangeMin.x && a.y >= rangeMin.y && a.z >= rangeMin.z &&
            a.x <= rangeMax.x && a.y <= rangeMax.y && a.z <= rangeMax.z;
}

bool inRange(vec a, ivec rangeMin, ivec rangeMax)
{
    return a.x >= rangeMin.x && a.y >= rangeMin.y && a.z >= rangeMin.z &&
            a.x <= rangeMax.x && a.y <= rangeMax.y && a.z <= rangeMax.z;
}

void printIvec(ivec n)
{
    printf("%i,%i,%i \n", n.x, n.y, n.z);
}

void printvec(vec n)
{
    printf("%.3f,%.3f,%.3f \n", n.x, n.y, n.z);
}
