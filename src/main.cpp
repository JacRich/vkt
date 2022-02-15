#include "globaldef.h"
#include "world.h"
#include "player.h"
#include "window.h"
#include "ivec.h"
#include "config.h"
#include "hud.h"
#include "cmesh.h"
#include "texture_merge.h"

float lastTime, deltaTime;
unsigned long framecount;
float gameTime = 0.0f;

int main()
{
  // The order matters! Do not touch!
  config_init();
  create_tex_atlas();
  window_init();
  player_init();
  world_init();
  hud_init();

  while (!glfwWindowShouldClose(window))
  {
    float time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime = time;

    player_tick();
    window_tick();
    meshing_tick();
    world_tick();
    hud_tick();
    lights_tick();

    framecount++;
    gameTime += deltaTime;
  }

  player_terminate();
  world_terminate();
  meshing_terminate();
  window_terminate();
  return 0;
}
