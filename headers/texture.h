#ifndef TEXTURE_H
#define TEXTURE_H

#include "globaldef.h"

using texture_t = uint32_t;

texture_t texture_make(char const *imagePath);

#endif