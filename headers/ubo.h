#ifndef UBO_H
#define UBO_H

#include "globaldef.h"

struct ubo_t {
    uint handle;
    uint size;
};

ubo_t ubo_make(uint size, uint binding);

void ubo_set(ubo_t ubo, void *data);

void ubo_set(ubo_t ubo, void *data, int offset);

extern ubo_t ubo_lights;

#endif