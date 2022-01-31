#include "lua_bindings.h"
#include "player.h"
#include "globaldef.h"

#include "phys.h"

#include "torch.h"
#include "player.h"
#include "veng.h"
#include "perlin.h"
#include "config.h"

int l_move_player(lua_State *L){
    double x = luaL_checknumber(L, 1);  /* get argument 1*/
    double y = luaL_checknumber(L, 2);  /* get argument 2*/
    double z = luaL_checknumber(L, 3);  /* get argument 3*/
    player.vel += vec{x, y,z}; /*move player by adding velocity*/
    return 0; /*number of arguments the function returns*/
    /*0 means no arguments are returned*/
}

int l_glfw_poll_events(lua_State *L){
    glfwPollEvents();
    return 0;
}

int l_destroy_block_look(lua_State *L){
    destroy();
    return 0;
}

int l_place_block_look(lua_State *L){
    place();
    return 0;
}

int l_get_key(lua_State *L){
    int key_check = luaL_checkinteger(L, 1);
    lua_pushinteger(L, glfwGetKey(window, key_check)); //return keypress
    return 1;
}

int l_set_torch_fuel(lua_State *L){
    double fuel = luaL_checknumber(L, 1);
    torch.fuel = fuel; //return keypress
    return 0;
}

int l_get_player_pos(lua_State *L){
    lua_createtable(L, 0, 3);
    lua_pushnumber(L, player.pos.x);
    lua_setfield(L, -2, "x");
    
    lua_pushnumber(L, player.pos.y);
    lua_setfield(L, -2, "y");
    
    lua_pushnumber(L, player.pos.z);
    lua_setfield(L, -2, "z");

    return 1;
}

void lua_bindings_init(lua_State *L){
    lua_pushcfunction(L, l_move_player);
    lua_setglobal(L, "move_player");

    lua_pushcfunction(L, l_glfw_poll_events);
    lua_setglobal(L, "glfw_poll_events");

    lua_pushcfunction(L, l_destroy_block_look);
    lua_setglobal(L, "destroy_block_look");

    lua_pushcfunction(L, l_place_block_look);
    lua_setglobal(L, "place_block_look");

    lua_pushcfunction(L, l_get_key);
    lua_setglobal(L, "get_key");

    lua_pushcfunction(L, l_set_torch_fuel);
    lua_setglobal(L, "set_torch_fuel");

    lua_pushcfunction(L, l_get_player_pos);
    lua_setglobal(L, "get_player_pos");
}