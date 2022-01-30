#include "cursor.h"
#include "shader.h"
#include "render.h"

const uchar cursorVertData[] = 
{
  // Y+        
  1, 1, 1,   
  1, 1, 0,  

  1, 1, 0,
  0, 1, 0,   

  0, 1, 0,   
  0, 1, 1,

  0, 1, 1,   
  1, 1, 1,   
// X+                            
  1, 1, 1,
  1, 0, 1,

  1, 0, 1,
  1, 0, 0, 

  1, 0, 0,  
  1, 1, 0,

  1, 1, 0,  
  1, 1, 1,  
// Y-                                
  0, 0, 1,   
  0, 0, 0,

  0, 0, 0,   
  1, 0, 0,   

  1, 0, 0,  
  1, 0, 1,

  1, 0, 1,  
  0, 0, 1,  
// X-                                
  0, 0, 1,   
  0, 1, 1,

  0, 1, 1,   
  0, 1, 0,   

  0, 1, 0,   
  0, 0, 0,

  0, 0, 0,   
  0, 0, 1,   
// Z+                                
  1, 0, 1,   
  1, 1, 1,

  1, 1, 1,   
  0, 1, 1,   

  0, 1, 1,   
  0, 0, 1,

  0, 0, 1,   
  1, 0, 1,   
// Z-                                
  0, 0, 0,  
  0, 1, 0,

  0, 1, 0,  
  1, 1, 0,  

  1, 1, 0,     
  1, 0, 0,

  1, 0, 0,     
  0, 0, 0   
};


void cursor_snapto(cursor_t* cursor, vhit hit)
{
  vec dir = (hit.pos - hit.posLast) * 1.0f;
  float dot = glm::dot(dir, {1.0f, 1.0f, 1.0f});

  if(dot > 0){
    cursor->mesh->pos = hit.pos - (vec{dir.x, dir.y,  dir.z} * (float)cursor->size);
  }
  else{
    cursor->mesh->pos = hit.posLast;
  }
}

void cursor_embed(cursor_t* cursor, vhit hit)
{
  vec dir = (hit.pos - hit.posLast) * 1.0f;
  float dot = glm::dot(dir, {1.0f, 1.0f, 1.0f});
    
  if(dot > 0){
    cursor->mesh->pos = hit.pos;
  }
  else{
    cursor->mesh->pos = hit.posLast + (vec{dir.x, dir.y,  dir.z} * (float)cursor->size);
  }
}

void cursor_center(cursor_t* cursor, vhit hit)
{
  cursor->mesh->pos = {hit.pos.x - (cursor->size / 2), hit.pos.y - (cursor->size / 2), hit.pos.z - (cursor->size / 2) };
}

void cursor_snap_chunk(cursor_t* cursor, chunk_t* chunk)
{
  cursor->mesh->pos = chunk->pos;
}

void cursor_grow(cursor_t* cursor)
{
  if(cursor->size > 25){
    return;
  }
  cursor->size++;
  cursor->mesh->scale = vec{float(cursor->size) + 0.01f};
}

void cursor_shrink(cursor_t* cursor)
{
  if(cursor->size <= 3){
    return;
  }
  cursor->size--;
  cursor->mesh->scale = vec{float(cursor->size) + 0.01f};
}

void cursor_setcolor(cursor_t* cursor, vec4 color)
{
  cursor->mesh->color = color;
}

void cursor_init(cursor_t* cursor)
{
  cursor->mesh = render_addmesh();

  // Set mesh params
  cursor->mesh->shader = sh_cursor;
  cursor->mesh->vertcount = 48;
  cursor->mesh->primtype = GL_LINES;
  cursor->mesh->scale = vec{cursor->size + 0.01f};

  uint vbo;
  glGenVertexArrays(1, &cursor->mesh->vao);
  glGenBuffers     (1, &vbo);
  
  glBindVertexArray(cursor->mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertData), cursorVertData, GL_STATIC_DRAW);

  // Stride = byte size of one vertex
  const int stride = 3;

  // Mark Vertex Position data in the buffer           
  const int aPos = 1;
  glEnableVertexAttribArray(aPos);
  glVertexAttribIPointer(aPos, 3, GL_UNSIGNED_BYTE, stride, (const void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}