#ifndef SHADER_H
#define SHADER_H
#include "globaldef.hpp"

void shader_make(uint* handle, char const* vertPath, char const* fragPath);
void shader_make(uint* handle, char const* vertPath, char const* fragPath, char const* geoPath);

void shader_setInt  (uint handle, const char* name, int value);
void shader_setFloat(uint handle, const char* name, float value);
void shader_setVec3(uint handle, const char* name, vec value);
void shader_setVec4(uint handle, const char* name, vec4 value);
void shader_setMat4(uint handle, const char* name, float* value);


#endif