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
#include <pthread.h>   
extern GLFWwindow* window; // From render
extern torch_t torch;

player_t player;

crosshair_t crosshair;

float lastTime, deltaTime;
unsigned long  framecount;

#define MAX_ENTS 10
entity_t entities[MAX_ENTS];
int entcount = 0;


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

  pthread_t thread;
  pthread_create(&thread, NULL, veng_terminate, NULL);
  pthread_join(thread, NULL);
  //veng_terminate();

  render_terminate();
  return 0;
}
