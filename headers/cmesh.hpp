#ifndef CMESH_H
#define CMESH_H
#include "globaldef.hpp"
#include "chunk.hpp"

#define BUFFER_SIZE 300000

struct cmesh_t
{
  chunk_t* chunk = NULL;

  uint vao, vbo;
  int facecount = 0;
  int vertcount = 0;

  uchar vertdata[BUFFER_SIZE];
  bool needToSend = false;
};

void cmesh_build(cmesh_t* mesh);
void cmesh_send (cmesh_t* mesh);

void cmesh_init(cmesh_t* mesh);

#endif
