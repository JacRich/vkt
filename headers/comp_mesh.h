#ifndef MESH_H
#define MESH_H

#include "globaldef.h"
#include "shader.h"
#include "texture.h"
#include "comp_transform.h"

#define DF_DEFAULT  0
#define DF_NO_DRAW  0x01
#define DF_NO_DEPTH 0x02
#define DF_NO_CULL  0x04

#define DF_HUD      DF_NO_DEPTH


struct mesh_t 
{
  uint8 drawflags = 0;
  int polymode = GL_FILL;

  uint vao = 0;
  uint vertcount = 0;
  
  shader_t shader = sh_item;
  texture_t texture = tex_atlas;

  vec4 color = vec4{1.0f, 0.5f, 0.1f, 1.0f};

  int customAttrib = 1;
};

mat4   mesh_makematrix(mesh_t *mesh, transform_t *transform);
mesh_t mesh_load(char const *path, shader_t shader, texture_t *texture, int drawflags);
void   meshes_tick();

#endif
