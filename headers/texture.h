#ifndef TEXTURE_H
#define TEXTURE_H
#include "globaldef.h"

using texture_t = uint32_t;

void texture_make(texture_t* handle, char const* imagePath);

#endif