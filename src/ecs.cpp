#include "ecs.h"

ent_tag_t ent_tags[MAX_ENTITIES];
components_t components;


entity_t entity_add(uint16_t tags) 
{
  for (int i = 0; i < MAX_ENTITIES; i++) {
    // Find an un-used entity, return it's index
    if (!ent_tags[i].isValid) {
      ent_tags[i].isValid = true;
      ent_tags[i].tags = tags;
      return i;
    }
  }
  printf("TOO MANY ENTITIES!\n");
  exit(0);
}

void entity_remove(entity_t index) 
{
  ent_tags[index].isValid = false;
  ent_tags[index].tags = 0;
}

bool entity_hasTags(entity_t entity, uint16_t tags) 
{
  return (ent_tags[entity].tags & tags) == tags;
}

void ecs_init() 
{

}
