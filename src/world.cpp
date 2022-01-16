#include "globaldef.hpp"
#include "veng.hpp"
#include "player.hpp"
#include "cursor.hpp"
#include "render.hpp"
#include "ivec.hpp"
#include "crosshair.hpp"
#include "perlin.hpp"
#include "torch.hpp"
#include "entity.hpp"
#include "config.hpp"

#include <pthread.h>   
extern GLFWwindow* window; // From render
extern torch_t torch;

player_t player;
crosshair_t crosshair;
config_t config;

float lastTime, deltaTime;
unsigned long  framecount;

#define MAX_ENTS 10
entity_t entities[MAX_ENTS];
int entcount = 0;

// My broke-ass ECS lmao
void world_add_entity(entity_t** entity)
{
  if(entcount > MAX_ENTS){
    printf("Over Max Entities!\n");
    return;
  }
  *entity = &entities[entcount];
  entcount++;
}

int main()
{
  load_config(&config);
  render_init(); // Always first
  player_init(&player);
  veng_init  ();
  crosshair_init(&crosshair, 0.015f);
  
 
  while(!glfwWindowShouldClose(window))
  {
    float time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime  = time;

    // Run Tick Function for all Entities
    for(int i = 0; i < entcount; i++){
      if(!entities[i].tick){
        continue;
      }
      entities[i].func_tick(entities[i].owner);
    }
    
    render_tick();
    veng_tick  ();
    framecount++;
  }

  player_terminate(&player); 
  veng_terminate  ();
  render_terminate();
  return 0;
}
