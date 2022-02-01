#ifndef MESH_H
#define MESH_H
#include "globaldef.h"
#include "shader.h"
#include "texture.h"

#define DF_VIS        0x01
#define DF_DEPTH_TEST 0x02

struct mesh_t
{
  uchar drawflags = DF_VIS | DF_DEPTH_TEST; 
  uchar primtype  = GL_TRIANGLES;

  uint vao = 0;
  uint vertcount = 0;
  shader_t shader   = sh_item;
  texture_t texture = tex_atlas;

  vec  pos;
  vec  scale = {1.0f,1.0f,1.0f};

  vec4 color = vec4{1.5f, 0.2f, 0.5f, 1.0f};

  float rotation = 0.0f;
  int  customAttrib = 1;
};

mat4 mesh_makematrix(mesh_t* mesh);
mesh_t mesh_load_obj(char const* path);

#endif
