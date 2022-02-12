#ifndef CMESH_H
#define CMESH_H

#include "globaldef.h"
#include "region.h"

#define BUFFER_SIZE 1750000

struct cmesh_t 
{
  bool needsend = false;
  chunk_t *chunk = NULL;

  uint vao, vbo;
  int facecount = 0;
  int vertcount = 0;

  uint8 vertdata[BUFFER_SIZE];
};

void cmesh_draw();
void meshing_init(region_t regions[REGION_COUNT]);

void cmeshes_tick();
void meshing_terminate();

#endif
