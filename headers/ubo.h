#ifndef UBO_H
#define UBO_H
#include "globaldef.h"

struct ubo_t
{
  uint handle;
  uint size  ;
};

ubo_t ubo_make(uint size, uint binding) 
{
  ubo_t ubo;
  ubo.size = size;
  glGenBuffers(1, &ubo.handle);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo.handle);
  glBufferData(GL_UNIFORM_BUFFER, ubo.size, (const void*)0, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo.handle);
  return ubo;
}

void ubo_set(ubo_t ubo, void* data)
{
  glBindBuffer   (GL_UNIFORM_BUFFER, ubo.handle);
  glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)0, ubo.size, data);
}



#endif