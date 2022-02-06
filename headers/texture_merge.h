#ifndef TEXTURE_MERGE_H
#define TEXTURE_MERGE_H

// basilicon - 02/06/2022
// Find all 16x16 textures in gamedata directory
// merge them into one 256x256 texture atlas

#include "globaldef.h"
#include <experimental/filesystem>
#include <math.h>

void create_tex_atlas();

#endif