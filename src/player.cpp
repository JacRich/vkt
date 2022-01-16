#include "globaldef.hpp"
#include "player.hpp"
#include "veng.hpp"
#include "render.hpp"
#include "torch.hpp"
#include "world.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>

#include "config.hpp"
extern GLFWwindow* window; // From render

extern player_t player;
extern config_t config;

cursor_t cursor, cursorRange;

// From world
extern float deltaTime;

bool   firstMouse = true;
double lastX, lastY;

bool inputingBlockId;
char ibuffer[3];
char buffer [3];
int size = 0;

extern torch_t torch;

static void player_set_selection(uchar value) 
{
  assert(value < 255 && value > 0);

  player.active = value;
}

static void player_set_inputmode(int mode)
{
  assert(mode >= 0 && mode <= 3);

  player.inputMode = mode;
}

static int load_player(player_t* player)
{
  if(access("gamedata/savedata/player", F_OK) != 0){
    printf("Failed to load player data, falling back to default\n");
    return 0;
  }

  FILE* file;
  file = fopen("gamedata/savedata/player", "rb");
  assert(file != NULL);
  
  player_t buffer;

  fread((void*)&buffer, sizeof(player_t), 1, file);

  player->pos = buffer.pos;

  fclose(file);
  return 1;
}

static void save_player(player_t* player)
{
  FILE* file;
  file = fopen("gamedata/savedata/player", "wb");

  fwrite((const void*)player, sizeof(player_t), 1, file);

  fclose(file);
}

static void key_input_edit(int key)
{
  if(inputingBlockId)
  {
    // Is key a number key?
    if(key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
    {
      int inputNumber = key - GLFW_KEY_0;
      ibuffer[size] = inputNumber;
      size++;
    }
    if(size > 2)
    {
      for(int i = 0; i < 3; i++)
      {
        sprintf(&buffer[i],"%i", ibuffer[i]);
      }
      long number;
      number = strtol(buffer, NULL, 10);
      printf("%li\n", number);
      size = 0;
      player_set_selection((uchar)number);
      inputingBlockId = false;
    }   
  }
  
  switch(key)
  {
    case(GLFW_KEY_ESCAPE):
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;

    case(GLFW_KEY_1):
    player_set_inputmode(IM_BLOCKS);
    break;

    case(GLFW_KEY_2):
    player_set_inputmode(IM_BLOCKS_RANGE_FILL);
    break;

    case(GLFW_KEY_3):
    player_set_inputmode(IM_BLOCKS_RANGE_REPLACE);
    break;

    case(GLFW_KEY_F11):
    {
      if(glfwGetWindowMonitor(window) == nullptr)
      {
        auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, vidMode->width, vidMode->height, 60);
      }
      else
      {
        glfwSetWindowMonitor(window, nullptr, 0, 0, 1280, 720, 60);
      } 
    }
    break;

    // Free/Lock mouse cursor
    case(GLFW_KEY_GRAVE_ACCENT):
    {
      int newMode = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED?  GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED; 
      glfwSetInputMode(window, GLFW_CURSOR, newMode);
    }
    break;

    case(GLFW_KEY_TAB):
    {
	    inputingBlockId = !inputingBlockId;
      size = 0;	
    }
    break;
  }
}

static void key_input_walk(int key)
{
  switch(key)
  {
    case(GLFW_KEY_ESCAPE):
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;

    case(GLFW_KEY_1):
    player_set_inputmode(IM_BLOCKS);
    break;

    case(GLFW_KEY_2):
    player_set_inputmode(IM_BLOCKS_RANGE_FILL);
    break;

    case(GLFW_KEY_3):
    player_set_inputmode(IM_BLOCKS_RANGE_REPLACE);
    break;

    case(GLFW_KEY_KP_0):
    save_region(find_region(player.pos));
    break;

    case(GLFW_KEY_F11):
    {
      if(glfwGetWindowMonitor(window) == nullptr)
      {
        auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, vidMode->width, vidMode->height, 60);
      }
      else
      {
        glfwSetWindowMonitor(window, nullptr, 0, 0, 1280, 720, 60);
      } 
    }
    break;

    // Free/Lock mouse cursor
    case(GLFW_KEY_GRAVE_ACCENT):
    {
      int newMode = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED?  GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED; 
      glfwSetInputMode(window, GLFW_CURSOR, newMode);
    }
    break;
  }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if(action != GLFW_PRESS) {
    return;
  }
  // Switch flying / walking
  if(key == GLFW_KEY_SPACE) {
    player.moveMode = !player.moveMode;
  }

  switch(player.moveMode)
  {
    case(MM_WALK):
    key_input_walk(key); 
    break;

    case(MM_FLY):
    key_input_edit(key);
    break;
  }
}

static void ms_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(yoffset > 0){
    cursor_grow(&cursorRange);
  }
  else{
    cursor_shrink(&cursorRange);
  }
}

static void ms_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if(action != GLFW_PRESS || hit.state != HIT_TRUE)
  {
    return;
  }

  if(button == GLFW_MOUSE_BUTTON_MIDDLE)
  {
    player_set_selection(*hit.voxel);
    return;
  }

  if(button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    switch(player.inputMode)
    {     
      case(IM_BLOCKS):
      {        
        veng_change_voxel(PICK_NORMAL, hit, player.active);
      }      
      break;

      case(IM_BLOCKS_RANGE_FILL):
      {
        veng_change_withcursor(&cursorRange, player.active, HIT_FALSE);
      }
      break;

      case(IM_BLOCKS_RANGE_REPLACE):
      {
        veng_change_withcursor(&cursorRange, player.active, HIT_TRUE);
      }
      break;
    } 
  }

  if(button == GLFW_MOUSE_BUTTON_LEFT)
  { 
    switch(player.inputMode)
    {     
      case(IM_BLOCKS):
      {
        veng_change_voxel(PICK_HIT, hit, 0); 
      }      
      break;

      case(IM_BLOCKS_RANGE_FILL):
      {
        veng_change_withcursor(&cursorRange, 0, HIT_TRUE);
      }

      case(IM_BLOCKS_RANGE_REPLACE):
      {
        veng_change_withcursor(&cursorRange, 0, HIT_TRUE);
      }
      break;
    } 
  }
}

static void player_rotate()
{
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  float xDelta = (xpos - lastX) * config.looksens;
  float yDelta = (lastY - ypos) * config.looksens;
  lastX = xpos;
  lastY = ypos;

  // if windows is focused, allow movement
  if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
  {
  player.yaw   += xDelta;
  player.pitch += yDelta;
  }

  // Clamp Pitch
  player.pitch = player.pitch > 89.0f? 89.0f : player.pitch < -89.0f? -89.0f: player.pitch;

  vec frontNew;
  frontNew.x = cos(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
  frontNew.y = sin(glm::radians(player.pitch));
  frontNew.z = sin(glm::radians(player.yaw)) * cos(glm::radians(player.pitch));
  player.front = glm::normalize(frontNew);
  player.right = glm::normalize(glm::cross(player.front, player.up));


  frontNew.x = cos(glm::radians(player.yaw)) * cos(glm::radians(0.0f));
  frontNew.y = sin(glm::radians(0.0f));
  frontNew.z = sin(glm::radians(player.yaw)) * cos(glm::radians(0.0f));
  player.frontMov = glm::normalize(frontNew);
  player.rightMov = glm::normalize(glm::cross(player.frontMov, player.up));
}

static vec make_clip(player_t* player)
{
  vec clipVector = {1, 1, 1};
  float marginNeg = 0.4f;
  float marginPos = 0.5f;

  for(int i = -3; i < 1; i++)
  {
    // Z+   
    if (player->vel.z > 0 && veng_sample_pos(player->pos + vec{0.0f, i, 1.0f}).state == HIT_TRUE)
    {
      if (player->pos.z - trunc(player->pos.z) > marginPos)
      {
        clipVector.z = 0;
      }
    }

    // Z-  
    if (player->vel.z < 0 && veng_sample_pos(player->pos + vec{0.0f, i, -1.0f}).state == HIT_TRUE)
    {
      if (player->pos.z - trunc(player->pos.z) < marginNeg)
      {
        clipVector.z = 0;
      }
    }

    // X+
    if (player->vel.x > 0 && veng_sample_pos(player->pos + vec{1.0f, i, 0.0f}).state == HIT_TRUE)
    {
      if (player->pos.x - trunc(player->pos.x) > marginPos)
      {
        clipVector.x = 0;
      }
    }

    // X-   
    if (player->vel.x < 0 && veng_sample_pos(player->pos + vec{-1.0f, i, 0.0f}).state == HIT_TRUE)
    {
      if (player->pos.x - trunc(player->pos.x) < marginNeg)
      {
        clipVector.x = 0;
      }
    }
  }

  // Y+
  if (player->vel.y > 0 && veng_sample_pos(player->pos + vec{0.0f, 1.0f, 0.0f}).state == HIT_TRUE)
  {
    if (player->pos.y - trunc(player->pos.y) > marginPos)
    {
      clipVector.y = 0;
    }
  }    

  // Y-
  if (player->vel.y < 0 && veng_sample_pos(player->pos + vec{0.0f, -4.0f, 0.0f}).state == HIT_TRUE)
  {
    if (player->pos.y - trunc(player->pos.y) < marginNeg)
    {
      clipVector.y = 0;
      player->grounded = true;
    }
  }
  else
  {
    player->grounded = false;
  }

  return clipVector;
}

static void player_move_walk(player_t* player)
{
  const vec zero = {0,0,0};
  vec movedir    = {0,0,0};

  movedir += glfwGetKey(window, GLFW_KEY_W) ? player->frontMov : zero;
  movedir += glfwGetKey(window, GLFW_KEY_S) ?-player->frontMov : zero;
  movedir += glfwGetKey(window, GLFW_KEY_A) ?-player->rightMov : zero;
  movedir += glfwGetKey(window, GLFW_KEY_D) ? player->rightMov : zero;

  // Apply mov vel  
  if(glm::length(player->vel) < 8.0f) {
    player->vel += (movedir * (player->accel * (player->grounded? 1.0f : 0.05f))) * deltaTime;  
  }
  player->vel += vec(0.0f, -15.0f, 0.0f) * deltaTime; // Gravity
  player->vel *= make_clip(player); // Try to prevent overlap(""try"")

  float friction = 1.0f / (1.0f + ((player->grounded? player->friction:player->friction * 0.001f) * deltaTime));
  player->vel *= friction;

  player->pos += player->vel * deltaTime;
}

static void player_move_fly(player_t* player)
{
  const vec zero = {0,0,0};
  vec movedir    = {0,0,0};

  movedir += glfwGetKey(window, GLFW_KEY_W) ? player->front : zero;
  movedir += glfwGetKey(window, GLFW_KEY_S) ?-player->front : zero;
  movedir += glfwGetKey(window, GLFW_KEY_A) ?-player->right : zero;
  movedir += glfwGetKey(window, GLFW_KEY_D) ? player->right : zero;

  // Apply mov vel  
  if(glm::length(player->vel) < 12.0f) {
    player->vel += (movedir * player->accel * 2.0f) * deltaTime;  
  }
  //player->vel *= make_clip(player); // Try to prevent overlap(""try"")

  float friction = 1.0f / (1.0f + (player->friction * deltaTime));
  player->vel *= friction;

  player->pos += player->vel * deltaTime;
}

static void cursor_move()
{
  vhit hit = veng_raycast(player.reach, player.pos, player.front);
  if(hit.state != HIT_TRUE)
  {
    cursor.mesh->drawflags = DF_DEPTH_TEST;
    cursorRange.mesh->drawflags = DF_DEPTH_TEST;
    return;
  }
  
  cursor_embed(&cursor, hit);
  cursor_centerOn(&cursorRange, hit);

  switch (player.inputMode)
  {
    case IM_BLOCKS:
    {
      cursor.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
      cursorRange.mesh->drawflags = DF_DEPTH_TEST;
      cursor_setcolor(&cursor, {0.9f, 0.4f, 0.0f, 1.0f});
      cursor_setcolor(&cursorRange, {0.8f, 0.1f, 0.5f, 1.0f});
    }
    break;

    case IM_BLOCKS_RANGE_FILL:
    {
      cursor.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
      cursorRange.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
      cursor_setcolor(&cursor, {0.9f, 0.4f, 0.0f, 1.0f});
      cursor_setcolor(&cursorRange, {0.9f, 0.1f, 0.05f, 1.0f});
    }
    break;

    case IM_BLOCKS_RANGE_REPLACE:
    {
      cursor.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
      cursorRange.mesh->drawflags = DF_DEPTH_TEST | DF_VIS;
      cursor_setcolor(&cursor, {0.9f, 0.4f, 0.0f, 1.0f});
      cursor_setcolor(&cursorRange, {0.8f, 0.1f, 0.5f, 1.0f});
    }
    break;
  }
}

void player_tick(void* owner)
{
  player_t* player_ptr = (player_t*)owner;
  torch.pos = player_ptr->pos;
  player_rotate();

  switch(player.moveMode)
  {
    case(MM_WALK):
    player_move_walk(&player); 
    break;

    case(MM_FLY):
    player_move_fly(&player);
    break;
  }

  cursor_move();
  glfwPollEvents();
}

void player_init(player_t* player)
{
  load_player(player);

  world_add_entity(&player->entity);
  player->entity->func_tick = player_tick;
  player->entity->owner = player;

  torch_init(&torch);

  glfwGetCursorPos(window, &lastX, &lastY);

  cursor_init(&cursor);
  cursorRange.size = 5.0f;
  cursor_init(&cursorRange);
  cursorRange.size = 5.0f;
  

  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetMouseButtonCallback(window, ms_button_callback);
  glfwSetScrollCallback     (window, ms_scroll_callback);
  glfwSetKeyCallback        (window, key_callback      );
}

void player_terminate(player_t* player)
{
  save_player(player);
}


