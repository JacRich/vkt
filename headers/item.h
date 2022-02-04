#ifndef ITEM_H
#define ITEM_H

#include "globaldef.h"
#include "mesh.h"
#include "thinker.h"

struct item_t {
    mesh_t *mesh;
    thinker_t *thinker;
};

void item_init(item_t *item, vec pos);


#endif