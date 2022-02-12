#include "globaldef.h"
#include "veng.h"
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
  veng_init();
  hud_init();

  while (!glfwWindowShouldClose(window))
  {
    float time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime = time;

    
    player_tick();
    render_tick();
    cmeshes_tick();
    veng_tick();
    hud_tick();
    lights_tick();
    framecount++;
    gameTime += deltaTime;
  }

  player_terminate();
  veng_terminate();
  meshing_terminate();
  render_terminate();
  return 0;
}
