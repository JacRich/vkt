#ifndef SHADER_H
#define SHADER_H
#include "globaldef.h"

using shader_t = uint32_t;

shader_t shader_make(char const* vertPath, char const* fragPath);
void shader_setInt   (shader_t  handle, const char* name, int value);
void shader_setFloat (shader_t  handle, const char* name, float value);
void shader_setVec3  (shader_t  handle, const char* name, vec value);
void shader_setVec4  (shader_t  handle, const char* name, vec4 value);
void shader_setMat4  (shader_t  handle, const char* name, float* value);


#endif