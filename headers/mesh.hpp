#ifndef MESH_H
#define MESH_H
#include "globaldef.hpp"
#include "shader.hpp"

#define DF_VIS        0x01
#define DF_DEPTH_TEST 0x02

struct mesh_t
{
  uchar drawflags = DF_VIS | DF_DEPTH_TEST; 
  uchar primtype  = GL_TRIANGLES;

  uint vao = 0;
  uint vertcount = 0;
  shader_t shader;

  vec  pos;
  vec  scale;
  vec4 color = vec4{1.5f, 0.2f, 0.5f, 1.0f};
  int  customAttrib = 1;
};

mat4 mesh_makematrix(mesh_t* mesh);


#endif
