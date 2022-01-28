#ifndef CMESH_H
#define CMESH_H
#include "globaldef.h"
#include "chunk.h"

#define BUFFER_SIZE 1750000

struct cmesh_t
{
  bool needsend = false;
  chunk_t* chunk = NULL;

  uint vao, vbo;
  int facecount = 0;
  int vertcount = 0;

  uchar vertdata[BUFFER_SIZE];
};

void cmesh_build(cmesh_t* mesh);
void cmesh_send (cmesh_t* mesh);

void cmesh_init(cmesh_t* mesh);

#endif
