#include "globaldef.hpp"
#include "veng.hpp"
#include "player.hpp"
#include "cursor.hpp"
#include "render.hpp"
#include "ivec.hpp"
#include "crosshair.hpp"
#include "entity.hpp"
#include "config.hpp"


float lastTime, deltaTime;
unsigned long  framecount;
float gameTime = 0.0f;

#define MAX_ENTS 10
entity_t entities[MAX_ENTS];
int entcount = 0;


void world_add_entity(entity_t** entity)
{
  if(entcount >= MAX_ENTS){
    printf("Over Max Entities!\n");
    return;
  }
  *entity = &entities[entcount];
  entcount++;
}

int main()
{ 
  config_init();  // Always first
  render_init();  // Always second 
  player_init();
  veng_init  ();
 
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
    
    player_tick();
    render_tick();
    veng_tick  ();
    framecount++;
    gameTime += deltaTime;
  }

  player_terminate();
  veng_terminate();
  render_terminate();
  return 0;
}
