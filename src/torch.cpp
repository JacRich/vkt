#include "torch.h"
#include "perlin.h"
#include "render.h"
#include "player.h"

static void torch_tick(void* owner)
{
  torch_t* torch = (torch_t*)owner;

  // Make torch flicker smoothly to emulate fire light
  float perlin = perlin_sampleOctave(gameTime, gameTime, gameTime,  0.35, 20);
  float offset = perlin * 0.55f;
  torch->light->color = (vec4{1.0f, 0.7f, 0.5f, 1.0} + offset) * (torch->fuel / torch->lifetime);
  torch->fuel -= deltaTime;
}

void torch_init(torch_t* torch)
{
  torch->thinker = thinker_add();
  torch->light   = render_add_light(player.pos);

  torch->thinker->owner     = torch;
  torch->thinker->func_tick = torch_tick;
}