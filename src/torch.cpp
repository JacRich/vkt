#include "torch.hpp"
#include "world.hpp"
#include "perlin.hpp"

extern float gameTime;

void torch_tick(void* owner)
{
  torch_t* torch = (torch_t*)owner;

  float perlin = perlin_sampleOctave(gameTime, gameTime, gameTime,  0.35, 20);
  float offset = perlin * 0.35f;
  torch->color = vec{1.0f, 0.7f, 0.5f} + offset;
}

void torch_init(torch_t* torch)
{
  world_add_entity(&torch->entity);
  torch->entity->func_tick = torch_tick;
  torch->entity->owner = torch;
}
