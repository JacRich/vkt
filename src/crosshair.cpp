#include "crosshair.h"
#include "render.h"

void crosshair_init(crosshair_t* crosshair, float width)
{
  float vertdata[] = {
   -width, 0.0f,
    width, 0.0f,
    0.0f,-width,
    0.0f, width
  };

  render_addmesh(&crosshair->mesh);
  crosshair->mesh->drawflags = DF_VIS;
  
  crosshair->mesh->shader = sh_cross;
  crosshair->mesh->vertcount = 4;
  crosshair->mesh->primtype = GL_LINES;
  

  glGenVertexArrays(1, &crosshair->mesh->vao);
  uint vbo;
  glGenBuffers(1, &vbo);

  // Allocate memory on the GPU for vertex data
  glBindVertexArray(crosshair->mesh->vao);
  glBindBuffer     (GL_ARRAY_BUFFER, vbo);
  glBufferData     (GL_ARRAY_BUFFER, sizeof(vertdata), vertdata, GL_STATIC_DRAW);

  // Indentify Vertex Position data in the buffer
  const int aPos = 0;
  const int stride = sizeof(float) * 2;
  glEnableVertexAttribArray(aPos);
  glVertexAttribPointer(aPos, 2, GL_FLOAT, false, stride, (const void *)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
