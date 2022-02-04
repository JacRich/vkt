#include "ecs.h"

ent_tag_t entities[MAX_ENTITIES];
components_t components;


entity_t entity_add(uint16_t tags) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        // Find an un-used entity
        if (!entities[i].isValid) {
            entities[i].isValid = true;
            entities[i].tags = tags;
            return i;
        }
    }
    printf("TOO MANY ENTITIES!\n");
    exit(0);
}

void entity_remove(entity_t index) {
    entities[index].isValid = false;
    entities[index].tags = 0;
}

bool entity_hasTags(entity_t entity, uint16_t tags) {
    return (entities[entity].tags & tags) == tags;
}

void ecs_init() {

}
