#include "hud.h"
#include "render.h"

const float hudCube[] =
{
// Y+  Pos         Normal
  0.5, 0.5, 0.5,   0, 1, 0,
  0.5, 0.5,-0.5,   0, 1, 0,
 -0.5, 0.5,-0.5,   0, 1, 0,

 -0.5, 0.5,-0.5,   0, 1, 0,
 -0.5, 0.5, 0.5,   0, 1, 0,
  0.5, 0.5, 0.5,   0, 1, 0,
// X+
  0.5, 0.5, 0.5,   1, 0, 0,
  0.5, -0.5, 0.5,  1, 0, 0,
  0.5, -0.5, -0.5, 1, 0, 0,

  0.5,-0.5,-0.5,   1, 0, 0,
  0.5, 0.5,-0.5,   1, 0, 0,
  0.5, 0.5, 0.5,   1, 0, 0,
// Y-
 -0.5, -0.5, 0.5,   0, -1, 0,
 -0.5, -0.5, -0.5,   0, -1, 0,
  0.5, -0.5, -0.5,   0, -1, 0,

  0.5, -0.5, -0.5,   0, -1, 0,
  0.5, -0.5, 0.5,   0, -1, 0,
 -0.5, -0.5, 0.5,   0, -1, 0,
// X-
  -0.5, -0.5, 0.5,   -1, 0, 0,
  -0.5, 0.5,  0.5,   -1, 0, 0,
  -0.5, 0.5, -0.5,   -1, 0, 0,

  -0.5, 0.5, -0.5,   -1, 0, 0,
  -0.5, -0.5, -0.5,   -1, 0, 0,
  -0.5, -0.5, 0.5,   -1, 0, 0,
// Z+
  0.5,-0.5, 0.5,   0, 0, 1,
  0.5, 0.5, 0.5,   0, 0, 1,
 -0.5, 0.5, 0.5,   0, 0, 1,

 -0.5, 0.5, 0.5,   0, 0, 1,
 -0.5,-0.5, 0.5,   0, 0, 1,
  0.5,-0.5, 0.5,   0, 0, 1,
// Z-
  -0.5, -0.5, -0.5,   0, 0, -1,
  -0.5, 0.5, -0.5,   0, 0, -1,
  0.5, 0.5, -0.5,   0, 0, -1,

  0.5, 0.5, -0.5,   0, 0, -1,
  0.5, -0.5, -0.5,   0, 0, -1,
  -0.5, -0.5, -0.5,   0, 0, -1
};


void hud_init(hud_t* hud)
{
  hud->mesh = render_addmesh();
  hud->mesh->shader = sh_hud;
  hud->mesh->rotation = 1.0f;
  
  hud->mesh->vertcount = 36;
  hud->mesh->pos = vec{2.6f, -1.3f, -3.0f};
  hud->mesh->scale *= 0.75f;
  hud->mesh->customAttrib = 5;
  hud->mesh->drawflags = DF_VIS;
  

  glGenVertexArrays(1, &hud->mesh->vao);
  uint vbo;
  glGenBuffers(1, &vbo);

  glBindVertexArray(hud->mesh->vao);
  glBindBuffer     (GL_ARRAY_BUFFER, vbo);
  glBufferData     (GL_ARRAY_BUFFER, sizeof(hudCube), hudCube, GL_STATIC_DRAW);

  const int stride = 6 * sizeof(float); // Byte size of one vertex
           
  const int aPos = 2;
  glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(0 * sizeof(float)));  
  glEnableVertexAttribArray(aPos);
  const int normalLocation = 3;
  glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(normalLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

