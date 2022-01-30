#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>
#include "globaldef.h"

#include "phys.h"

#include "torch.h"
#include "player.h"
#include "veng.h"
#include "perlin.h"
#include "config.h"



player_t player;
cursor_t cursor_single, cursor_range; 
double lastX, lastY;

torch_t torch;


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
    player.editor = !player.editor;
  }

  if(key == GLFW_KEY_ESCAPE){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if(key == GLFW_KEY_HOME){
    torch.fuel = torch.lifetime;
  }

  if(key == keys.jump && player.grounded){
    player.vel += vec{0, 6, 0};
  }

  if(key == keys.im_blocks){
    player.inputMode = IM_BLOCKS;
  }

  if(key == keys.im_range){
    player.inputMode = IM_BLOCKS_RANGE_FILL;
    cursor_setcolor(&cursor_range, {0.9f, 0.1f, 0.05f, 1.0f});
  }

  if(key == keys.im_range_replace){
    player.inputMode = IM_BLOCKS_RANGE_REPLACE;
    cursor_setcolor(&cursor_range, {0.8f, 0.1f, 0.5f, 1.0f});
  }

  if(key == keys.toggle_coll){
    config.editorcoll = !config.editorcoll;
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
    cursor_grow(&cursor_range);
  }
  else{
    cursor_shrink(&cursor_range);
  }
}

static void ms_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if (action != GLFW_PRESS || hit.state != HIT_TRUE){
    return;
  }

  if (button == mouse.pickblock){
    player.active = *hit.voxel;
  }
  if (button == GLFW_MOUSE_BUTTON_4){
    render_add_light(hit.posLast);
    
    coll_sphere_t* collider = phys_add_collider();
    collider->pos = hit.posLast;
  }
  

  if (button == mouse.place)
  {
    switch (player.inputMode)
    {
      case (IM_BLOCKS):   
      veng_change_voxel(hit, PICK_NORMAL, player.active);
      break;

      case (IM_BLOCKS_RANGE_FILL):
      veng_change_withcursor(&cursor_range, player.active, HIT_FALSE);
      break;

      case (IM_BLOCKS_RANGE_REPLACE):
      veng_change_withcursor(&cursor_range, player.active, HIT_TRUE);
      break;
    }
  }

  if (button == mouse.destroy)
  {
    switch (player.inputMode)
    {
      case (IM_BLOCKS):
      veng_change_voxel(hit, PICK_HIT, 0);
      break;

      case (IM_BLOCKS_RANGE_FILL):
      veng_change_withcursor(&cursor_range, 0, HIT_TRUE);
      break;

      case (IM_BLOCKS_RANGE_REPLACE):
      veng_change_withcursor(&cursor_range, 0, HIT_TRUE);
      break;
    }
  }
}

static void ms_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
  float xDelta = (xpos - lastX) * config.looksens;
  float yDelta = (lastY - ypos) * config.looksens;
  lastX = xpos;
  lastY = ypos;
  player.yaw   += xDelta;
  player.pitch += yDelta;
  player.pitch = player.pitch > 89.0f ? 89.0f : player.pitch < -89.0f ? -89.0f : player.pitch;

  // Make direction vectors for flying
  vec frontNew;
  frontNew.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
  frontNew.y = sin(glm::radians(player.pitch));
  frontNew.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
  player.front = glm::normalize(frontNew);
  player.right = glm::normalize(glm::cross(player.front, player.up));

  // Make direction vectors for walking
  frontNew.x = cos(glm::radians(player.yaw)) * cos(glm::radians(0.0f));
  frontNew.y = sin(glm::radians(0.0f));
  frontNew.z = sin(glm::radians(player.yaw)) * cos(glm::radians(0.0f));
  player.front_walk = glm::normalize(frontNew);
  player.right_walk = glm::normalize(glm::cross(player.front_walk, player.up));
}


static vec make_clip(player_t *player)
{
  vec clipVector = {1, 1, 1};
  float marginNeg = 0.4f;
  float marginPos = 0.5f;

  for (int i = -2; i < 1; i++)
  {
    // Z+
    if (player->vel.z > 0 && veng_find_voxel(player->pos + vec{0.0f, i, 1.0f}).state == HIT_TRUE)
    {
      if (player->pos.z - trunc(player->pos.z) > marginPos)
      {
        clipVector.z = 0;
      }
    }

    // Z-
    if (player->vel.z < 0 && veng_find_voxel(player->pos + vec{0.0f, i, -1.0f}).state == HIT_TRUE)
    {
      if (player->pos.z - trunc(player->pos.z) < marginNeg)
      {
        clipVector.z = 0;
      }
    }

    // X+
    if (player->vel.x > 0 && veng_find_voxel(player->pos + vec{1.0f, i, 0.0f}).state == HIT_TRUE)
    {
      if (player->pos.x - trunc(player->pos.x) > marginPos)
      {
        clipVector.x = 0;
      }
    }

    // X-
    if (player->vel.x < 0 && veng_find_voxel(player->pos + vec{-1.0f, i, 0.0f}).state == HIT_TRUE)
    {
      if (player->pos.x - trunc(player->pos.x) < marginNeg)
      {
        clipVector.x = 0;
      }
    }
  }

  // Y+
  if (player->vel.y > 0 && veng_find_voxel(player->pos + vec{0.0f, 1.0f, 0.0f}).state == HIT_TRUE)
  {
    if (player->pos.y - trunc(player->pos.y) > marginPos)
    {
      clipVector.y = 0;
    }
  }

  // Y-
  if (player->vel.y < 0 && veng_find_voxel(player->pos + vec{0.0f, -3.0f, 0.0f}).state == HIT_TRUE)
  {
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

static void player_move_walk(player_t *player)
{
  const vec zero = {0, 0, 0};
  vec movedir    = {0, 0, 0};

  movedir += glfwGetKey(window, keys.forward) ?  player->front_walk : zero;
  movedir += glfwGetKey(window, keys.backward)? -player->front_walk : zero;
  movedir += glfwGetKey(window, keys.left   ) ? -player->right_walk : zero;
  movedir += glfwGetKey(window, keys.right  ) ?  player->right_walk : zero;

  // Apply mov vel
  if (glm::length(player->vel) < 8.0f){
    player->vel += (movedir * (config.moveSpeed * (player->grounded ? 1.0f : 0.05f))) * deltaTime;
  }
  // Gravity
  player->vel += vec(0.0f, -15.0f, 0.0f) * deltaTime;
 
  // Collision detection & response, needs work
  player->vel *= make_clip(player);
  vhit ray = veng_raycast(4, player->pos, {0.0f, -1.0f, 0.0f});
  if(ray.distance < 3.4f){
    player->pos += vec{0.0f, 0.01f,0.0f};
  }

  float friction = 1.0f / (1.0f + ((player->grounded ? config.friction : config.friction * 0.001f) * deltaTime));
  player->vel *= friction;

  player->pos += player->vel * deltaTime;
}

static void player_move_fly(player_t *player)
{
  const vec zero = {0, 0, 0};
  vec movedir    = {0, 0, 0};

  movedir += glfwGetKey(window, keys.forward ) ?  player->front : zero;
  movedir += glfwGetKey(window, keys.backward) ? -player->front : zero;
  movedir += glfwGetKey(window, keys.left    ) ? -player->right : zero;
  movedir += glfwGetKey(window, keys.right   ) ?  player->right : zero;

  // Apply mov vel
  if (glm::length(player->vel) < 12.0f){
    player->vel += (movedir * 120.0f) * deltaTime;
  }

  // Collision detection and response
  if (config.editorcoll){
    player->vel *= make_clip(player);
  }

  float friction = 1.0f / (1.0f + (config.friction * deltaTime));
  player->vel *= friction;

  player->pos += player->vel * deltaTime;
}

void player_tick()
{
  glfwPollEvents();
  
  torch.light->pos = vec_to_vec4(player.pos);

  if(player.editor){
    player_move_fly(&player);
  }
  else{
    player_move_walk(&player);
  }
  
  // Move Cursors
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if(hit.state != HIT_TRUE){
    cursor_single.mesh->drawflags = DF_DEPTH_TEST;
    cursor_range.mesh->drawflags  = DF_DEPTH_TEST;
    return;
  }
  cursor_embed (&cursor_single, hit);
  cursor_center(&cursor_range, hit);

  if(player.inputMode == IM_BLOCKS){
    cursor_single.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
    cursor_range.mesh->drawflags = 0;
  }
  else{
    cursor_single.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
    cursor_range.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
  }
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
  torch_init (&torch);

  cursor_init(&cursor_single);
  cursor_range.size = 5.0f;
  cursor_init(&cursor_range);


  glfwGetCursorPos(window, &lastX, &lastY);
  // Grab cursor
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

  glfwSetMouseButtonCallback(window, ms_button_callback);
  glfwSetScrollCallback(window, ms_scroll_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, ms_pos_callback);
}

void player_terminate()
{
  save_player(&player);
}


