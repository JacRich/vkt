#ifndef MESH_H
#define MESH_H
#include "globaldef.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"


#define DF_NO_DRAW  0x01
#define DF_NO_DEPTH 0x02

struct mesh_t
{
  uchar drawflags = 0; 
  int polymode = GL_FILL;

  uint vao = 0;
  uint vertcount = 0;
  shader_t  shader  = sh_item;
  texture_t texture = tex_atlas;

  vec4 color = vec4{1.5f, 0.2f, 0.5f, 1.0f};

  float rotation = 0.0f;
  int  customAttrib = 1;
};

mat4 mesh_makematrix(mesh_t* mesh, transform_t* transform);
mesh_t mesh_load_obj(char const* path, shader_t shader, texture_t* texture, int drawflags);
void meshes_draw();

#endif
