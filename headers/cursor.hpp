#ifndef CURSOR_H
#define CURSOR_H
#include "globaldef.hpp"
#include "vhit.hpp"
#include "mesh.hpp"

struct cursor_t
{
  mesh_t* mesh;

  int size = 1;
};

void cursor_snapto(cursor_t* cursor, vhit hit);
void cursor_embed(cursor_t* cursor, vhit hit);
void cursor_center(cursor_t* cursor, vhit hit);
void cursor_snap_chunk(cursor_t* cursor, chunk_t* chunk);

void cursor_grow(cursor_t* cursor);
void cursor_shrink(cursor_t* cursor);


void cursor_setcolor(cursor_t* cursor, vec4 color);

void cursor_draw(cursor_t* cursor, uint shader);
void cursor_init(cursor_t* cursor);

#endif