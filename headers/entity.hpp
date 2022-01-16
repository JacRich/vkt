#ifndef ENTITY_H
#define ENTITY_H
#include "globaldef.hpp"

struct entity_t
{
  bool tick = true; 
  void* owner;
  void (*func_tick)(void*);
};


#endif
