#ifndef ECS_H
#define ECS_H

#include "globaldef.h"
#include "comp_mesh.h"
#include "comp_transform.h"
#include "comp_light.h"

#define MAX_ENTITIES 100

#define C_MESH       0x01
#define C_TRANSFORM  0x02
#define C_LIGHT      0x04

using entity_t = uint32_t;

struct ent_tag_t 
{
  bool isValid = false;
  uint16 tags = 0;
};

struct components_t 
{
  mesh_t      meshes    [MAX_ENTITIES];
  transform_t transforms[MAX_ENTITIES];
  light_t     lights    [MAX_ENTITIES];
};

extern ent_tag_t ent_tags[MAX_ENTITIES];
extern components_t components;

entity_t entity_add(uint16 tags);

void entity_remove(entity_t index);

bool entity_hasTags(entity_t entity, uint16_t tags);
void* entity_get_comp(entity_t entity, uint16 componentType);

#endif