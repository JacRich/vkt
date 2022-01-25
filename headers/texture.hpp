#ifndef TEXTURE_H
#define TEXTURE_H
#include "globaldef.hpp"

using texture_t = uint32_t;

void texture_make(texture_t* handle, char const* imagePath);

#endif