#include "globaldef.h"
#include "veng.h"
#include "player.h"
#include "cursor.h"
#include "render.h"
#include "ivec.h"
#include "config.h"
#include "thinker.h"

#include "minilua.h"
#include "lua_bindings.h"

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
  
  /*------------------------LUA SETUP------------------------*/
    lua_State *L = luaL_newstate(); /*create lua instance*/
    if(L == NULL) /*check if pointer is null*/
        return -1; /*exit program if lua failed to start*/
    luaL_openlibs(L); /*open lua stdlib*/
    lua_bindings_init(L); /*initilize lua bindings so you can use C functions*/
    if (luaL_loadfile(L, "main.lua")){printf("Error {main.lua} not found. closing..."); return -1} /*load file*/
    lua_call(L, 0, 0); /*call the file*/
    lua_getglobal(L,"update"); /*push function to the stack*/
  /*---------------------------------------------------------*/
  
  
  while(!glfwWindowShouldClose(window))
  {
    float time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime  = time;
    
    player_tick ();
    
    /*call lua update function with deltaTime as argument*/
    lua_pushvalue(L,-1); /*get function again without searching for it with getglobal*/
    lua_pushnumber(L,deltaTime); /*pushing argument*/
    lua_call(L, 1, 0);  /*call function on stack (LuaInstance, arg num, return num)*/
    
    render_tick ();
    veng_tick   ();
    thinker_tick();
    framecount++;
    gameTime += deltaTime;
  }
  
  lua_close(L); /*close lua instance*/
  player_terminate();
  veng_terminate  ();
  render_terminate();
  return 0;
}
