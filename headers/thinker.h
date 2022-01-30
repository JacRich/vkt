#ifndef THINKER_H
#define THINKER_H

struct thinker_t
{
  void (*func_tick)(void* owner);
  void* owner;
};

thinker_t* thinker_add();
void thinker_tick();

#endif