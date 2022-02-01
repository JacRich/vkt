#include "globaldef.h"
#include "thinker.h"

thinker_t thinkers[MAX_ENTS]; 
bool      inUse   [MAX_ENTS];
int thinker_count = 0;


thinker_t* thinker_add()
{
  thinker_count++;
  return &thinkers[thinker_count - 1];
}

void thinker_tick()
{
  for(int i = 0; i < thinker_count; i++){
    thinkers[i].func_tick(thinkers[i].owner);
  }
}