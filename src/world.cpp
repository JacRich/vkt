#include "globaldef.hpp"
#include "veng.hpp"
#include "player.hpp"
#include "cursor.hpp"
#include "render.hpp"
#include "ivec.hpp"
#include "entity.hpp"
#include "config.hpp"

float lastTime, deltaTime;
unsigned long  framecount;
float gameTime = 0.0f;

int main()
{ 
  // The order matters! Do not touch!
  config_init();
  render_init(); 
  player_init();
  veng_init  ();
 
  while(!glfwWindowShouldClose(window))
  {
    float time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime  = time;
    
    player_tick();
    render_tick();
    veng_tick  ();
    framecount++;
    gameTime += deltaTime;
  }

  player_terminate();
  veng_terminate  ();
  render_terminate();
  return 0;
}
