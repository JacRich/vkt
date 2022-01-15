#include "torch.hpp"
#include "world.hpp"
#include "perlin.hpp"

extern float lastTime, deltaTime;
extern unsigned long framecount;

void torch_tick(void* owner)
{
  torch_t* torch = (torch_t*)owner;

  float perlin = perlin_sampleOctave(framecount * 0.002, framecount * 0.002, framecount * 0.002,  0.35, 20);
  float offset = perlin * 0.35f;
  torch->color = vec{1.0f, 0.7f, 0.5f} + offset;
}

void torch_init(torch_t* torch)
{
  world_add_entity(&torch->entity);
  torch->entity->func_tick = torch_tick;
  torch->entity->owner = torch;
}
