#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>
#include "globaldef.h"

#include "player.h"
#include "veng.h"
#include "perlin.h"
#include "config.h"
#include "hud.h"
#include "ecs.h"


player_t player;
double lastX, lastY;

entity_t torch;
transform_t* torch_tran;


static void destroy()
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if (hit.state != HIT_TRUE){
    return;
  }
  veng_change_voxel(hit, PICK_HIT, 0);
}

static void place()
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if (hit.state != HIT_TRUE){
    return;
  }
  veng_change_voxel(hit, PICK_NORMAL, player.active);
}

static void pick()
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if (hit.state != HIT_TRUE){
    return;
  }
  player.active = *hit.voxel;
}

static void inputid(int key)
{
  // Someome who is better at coding plz clean this up
  static char ibuffer[3];
  static char buffer [3];
  static int  size = 0;
  if(glfwGetKey(window, keys.input_id)){
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9){
      int inputNum = key - GLFW_KEY_0;
      ibuffer[size] = inputNum;
      size++;
      if(size == 3){
        for (int i = 0; i < 3; i++){
          sprintf(&buffer[i], "%i", ibuffer[i]);
        }
        long number = strtol(buffer, NULL, 10);
        player.active = (uchar)number;
        size = 0;
      }
    }
    return;
  }
  else{
    size = 0;
  }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action != GLFW_PRESS){
    return;
  }
  inputid(key);

  // Switch flying / walking
  if (key == keys.switchmov){
    player.flying = !player.flying;
  }
  if(key == GLFW_KEY_KP_0){
    player.flyingColl = !player.flyingColl;
  }

  if(key == GLFW_KEY_ESCAPE){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if(key == GLFW_KEY_HOME){
    region_fill_value(veng_find_region(player.pos), 5);
  }

  if(key == GLFW_KEY_INSERT){
    region_fill_perlin(veng_find_region(player.pos));
  }

  if(key == keys.jump && player.grounded){
    player.vel += vec{0, 8, 0};
  }

  if(key == keys.toggle_fullbright){
    config.fullbright = !config.fullbright;
  }
  
  if(key == keys.toggle_fullscreen){
    if (glfwGetWindowMonitor(window) == nullptr){ // If not fullscreen
      auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, vidMode->width, vidMode->height, 60);
      glViewport(0, 0, vidMode->width, vidMode->height);
    }
    else{
      glfwSetWindowMonitor(window, nullptr, 0, 0, config.width, config.height, 60);
      glViewport(0, 0, config.width, config.height);
    }
  }
  
  if(key == keys.toggle_cursor){
    int newMode = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
    glfwSetInputMode(window, GLFW_CURSOR, newMode);
  }
}

static void ms_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  if (yoffset > 0){
    
  }
  else{
    
  }
}

static void ms_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if (action != GLFW_PRESS){
    return;
  }

  if (button == GLFW_MOUSE_BUTTON_4){
    entity_t newLight = entity_add(C_TRANSFORM | C_LIGHT);
    components.transforms[newLight].pos = hit.posLast;
  }

  if (button == mouse.place){
    place();
  }
  if (button == mouse.destroy){
    destroy();
  }  
  if (button == mouse.pickblock){
    pick();
  }
}

// This is beyond jank, too bad!
static vec make_clip(player_t* player)
{
  vec clipVector = {1, 1, 1};
  float marginNeg = 0.4f;
  float marginPos = 0.5f;

  for (int i = -2; i < 1; i++)
  {
    // Z+
    if (player->vel.z > 0 && veng_find_voxel(player->pos + vec{0.0f, i, 1.0f}).state == HIT_TRUE){
      if (player->pos.z - trunc(player->pos.z) > marginPos){
        clipVector.z = 0;
      }
    }

    // Z-
    if (player->vel.z < 0 && veng_find_voxel(player->pos + vec{0.0f, i, -1.0f}).state == HIT_TRUE){
      if (player->pos.z - trunc(player->pos.z) < marginNeg){
        clipVector.z = 0;
      }
    }

    // X+
    if (player->vel.x > 0 && veng_find_voxel(player->pos + vec{1.0f, i, 0.0f}).state == HIT_TRUE){
      if (player->pos.x - trunc(player->pos.x) > marginPos){
        clipVector.x = 0;
      }
    }

    // X-
    if (player->vel.x < 0 && veng_find_voxel(player->pos + vec{-1.0f, i, 0.0f}).state == HIT_TRUE){
      if (player->pos.x - trunc(player->pos.x) < marginNeg){
        clipVector.x = 0;
      }
    }
  }

  // Y+
  if (player->vel.y > 0 && veng_find_voxel(player->pos + vec{0.0f, 1.0f, 0.0f}).state == HIT_TRUE){
    if (player->pos.y - trunc(player->pos.y) > marginPos){
      clipVector.y = 0;
    }
  }

  // Y-
  if (player->vel.y < 0 && veng_find_voxel(player->pos + vec{0.0f, -3.0f, 0.0f}).state == HIT_TRUE){
    if (player->pos.y - trunc(player->pos.y) < marginNeg){
      clipVector.y = 0;
      player->grounded = true;
    }
  }
  else{
    player->grounded = false;
  }

  return clipVector;
}

static void player_move()
{
  vec front = player.flying? player.front : player.front_walk;
  vec right = player.flying? player.right : player.right_walk;

  const vec zero = {0,0,0};
  vec movedir    = {0,0,0};
  
  movedir += glfwGetKey(window, keys.forward ) ?  front : zero;
  movedir += glfwGetKey(window, keys.backward) ? -front : zero;
  movedir += glfwGetKey(window, keys.left    ) ? -right : zero;
  movedir += glfwGetKey(window, keys.right   ) ?  right : zero;

  // This dumbness is for avoiding an error where normalize returns NAN for zero length vectors
  movedir  = glm::length(movedir) > 0? glm::normalize(movedir) : zero;
  movedir *= config.moveSpeed;
  movedir *= deltaTime;

  if(player.flying)
  {
    player.vel += movedir;
    player.vel *= 1.0f / (1.0f + (config.friction * deltaTime)); // Friction
    if(player.flyingColl) player.vel *= make_clip(&player);
  }
  else
  {
    player.vel += vec{0,-16.0f, 0} * deltaTime;
    
    if(player.grounded)
    {
      player.vel += movedir;
      player.vel *= 1.0f / (1.0f + (config.friction * deltaTime)); // Friction
    }
    else
    {
      player.vel += movedir * 0.1f;
    }
    
    player.vel *= make_clip(&player);
  }

  player.pos += player.vel * deltaTime;
}


void player_tick()
{
  glfwPollEvents();
  int trunc_ms = int(gameTime * 1000);

  static double destroy_timeHeld = 0.0;
  if(glfwGetMouseButton(window, mouse.destroy)){
    destroy_timeHeld += deltaTime;
  }
  else{ destroy_timeHeld = 0.0; }
  if(destroy_timeHeld > 0.2 && trunc_ms % 5 == 0){
    destroy();
  }

  static double place_timeHeld = 0.0;
  if(glfwGetMouseButton(window, mouse.place)){
    place_timeHeld += deltaTime;
  }
  else{ place_timeHeld = 0.0; }
  if(place_timeHeld > 0.2 && trunc_ms % 5 == 0){ 
    place();
  }

  torch_tran->pos = player.pos;
  player_move();

  // Update direction vectors
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  float xDelta = (xpos - lastX) * config.looksens;
  float yDelta = (lastY - ypos) * config.looksens;
  lastX = xpos;
  lastY = ypos;
  player.yaw   += xDelta;
  player.pitch += yDelta;
  player.pitch = player.pitch > 89.0f ? 89.0f : player.pitch < -89.0f ? -89.0f : player.pitch;

  // Vectors for flying
  vec frontNew;
  frontNew.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
  frontNew.y = sin(glm::radians(player.pitch));
  frontNew.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
  player.front = glm::normalize(frontNew);
  player.right = glm::normalize(glm::cross(player.front, player.up));

  // Vectors for walking
  frontNew.x = cos(glm::radians(player.yaw)) * cos(glm::radians(0.0f));
  frontNew.y = sin(glm::radians(0.0f));
  frontNew.z = sin(glm::radians(player.yaw)) * cos(glm::radians(0.0f));
  player.front_walk = glm::normalize(frontNew);
  player.right_walk = glm::normalize(glm::cross(player.front_walk, player.up));
}


static void load_player(player_t* player)
{
  FILE *file;
  file = fopen("gamedata/savedata/player", "rb");
  if (file == NULL){
    printf("Failed to load player save, falling back to default\n");
    return;
  }
  player_t buff;
  fread((void*)&buff, sizeof(player_t), 1, file);
  player->pos = buff.pos;

  fclose(file);
}

static void save_player(player_t* player)
{
  FILE *file;
  file = fopen("gamedata/savedata/player", "wb");

  fwrite((const void*)player, sizeof(player_t), 1, file);

  fclose(file);
}

void player_init()
{
  load_player(&player);
  torch = entity_add(C_TRANSFORM | C_LIGHT);
  torch_tran = &components.transforms[torch];
  
  glfwGetCursorPos(window, &lastX, &lastY);
  // Grab cursor
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
  glfwSetMouseButtonCallback(window, ms_button_callback);
  glfwSetScrollCallback(window, ms_scroll_callback);
  glfwSetKeyCallback(window, key_callback);
}

void player_terminate()
{
  save_player(&player);
}


