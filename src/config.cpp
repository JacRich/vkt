#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <string.h>

config_t       config;
config_key_t   keys;
config_mouse_t mouse;


static void load_config(config_t* config) 
{
	FILE* file = fopen("config", "rb");
  if(file == NULL){
    printf("%s\n", "Failed to load config, falling back to default");
    return;
  }
  
  char linebuf[100];
  while(!feof(file)) 
	{
    fgets(linebuf, 100, file);
    // Is line a comment or empty?
    if(linebuf[0] == '#' || strlen(linebuf) < 4){
      continue;
    }

    int fullbright;
    if (sscanf(linebuf, " fullbright = %i", &fullbright)){ 
      config->fullbright = fullbright; 
    }

    int hideGUI;
    if (sscanf(linebuf, " hideGUI = %i", &hideGUI)){
        config->hideGUI = hideGUI;
    }

    int recordingMode;
    if (sscanf(linebuf, " recordingMode = %i", &recordingMode)){
        config->recordingMode = recordingMode;
    }

    int width;
    if (sscanf(linebuf, " width = %i", &width)){
      config->width = width;
    }

    int height;
    if (sscanf(linebuf, " height = %i", &height)){
      config->height = height;
    }
    
    float fov;
    if (sscanf(linebuf, " fov = %f", &fov)){
      config->fov = fov;
    }

    float movespeed;
    if (sscanf(linebuf, " movespeed = %f", &movespeed)){
      config->moveSpeed = movespeed;
    }

    float friction;
    if (sscanf(linebuf, " friction = %f", &friction)){
      config->friction = friction;
    }

    float renderdist;
    if (sscanf(linebuf, " renderdist = %f", &renderdist)){
      config->renderdist = renderdist;
    }

    float looksens;
    if (sscanf(linebuf, " looksens = %f", &looksens)){
      config->looksens = looksens;
    }

    vec spawnpos;
    if (sscanf(linebuf, " spawnpos = %f%f%f", &spawnpos.x,&spawnpos.y,&spawnpos.z)){
      config->spawnpos = spawnpos;
    }
  }

  fclose(file);
}

static void load_config_key(config_key_t* config)
{
  if(access("config", F_OK) != 0){      
    printf("%s\n", "Failed to load key config, falling back to default");
    return;
  }

	FILE *file = fopen("config_keys", "rb");
  char linebuf[100];
  
  while(!feof(file)) 
	{
    fgets(linebuf, 100, file);
    // Is line a comment or empty?
    if (linebuf[0] == '#' || strlen(linebuf) < 4){
      continue;
    }

    int buffer;
    if (sscanf(linebuf, " backward = %i", &buffer)){
      config->backward = buffer;
    }

    if (sscanf(linebuf, " forward = %i", &buffer)){
      config->forward = buffer;
    }

    if (sscanf(linebuf, " im_blocks = %i", &buffer)){
      config->im_blocks = buffer;
    }

    if (sscanf(linebuf, " im_range = %i", &buffer)){
      config->im_range = buffer;
    }

    if (sscanf(linebuf, " im_range_replace = %i", &buffer)){
      config->im_range_replace = buffer;
    }

    if (sscanf(linebuf, " input_id = %i", &buffer)){
      config->input_id = buffer;
    }

    if (sscanf(linebuf, " jump = %i", &buffer)){
      config->jump = buffer;
    }

    if (sscanf(linebuf, " left = %i", &buffer)){
      config->left = buffer;
    }

    if (sscanf(linebuf, " right = %i", &buffer)){
      config->right = buffer;
    }

    if (sscanf(linebuf, " toggle_coll = %i", &buffer)){
      config->toggle_coll = buffer;
    }

    if (sscanf(linebuf, " toggle_cursor = %i", &buffer)){
      config->toggle_cursor = buffer;
    }

    if (sscanf(linebuf, " toggle_fullbright = %i", &buffer)){
      config->toggle_fullbright = buffer;
    }

    if (sscanf(linebuf, " toggle_fullscreen = %i", &buffer)){
      config->toggle_fullscreen = buffer;
    }

    if (sscanf(linebuf, " switchmov = %i", &buffer)){
      config->switchmov = buffer;
    }

    if (sscanf(linebuf, " toggle_gui = %i", &buffer)){
        config->toggle_gui = buffer;
    }

    if (sscanf(linebuf, " toggle_recording = %i", &buffer)){
        config->toggle_recording = buffer;
    }
  }

  fclose(file);
}

static void load_config_mouse(config_mouse_t* config)
{
  if(access("config", F_OK) != 0){      
    printf("%s\n", "Failed to load mouse config, falling back to default");
    return;
  }

	FILE *file = fopen("config_mouse", "rb");
  char linebuf[100];
  
  while(!feof(file)) 
	{
    fgets(linebuf, 100, file);
    // Is line a comment or empty?
    if (linebuf[0] == '#' || strlen(linebuf) < 4){
      continue;
    }

    int buffer;
    if (sscanf(linebuf, " place = %i", &buffer)){
      config->place = buffer;
    }
    if (sscanf(linebuf, " destroy = %i", &buffer)){
      config->destroy = buffer;
    }
    if (sscanf(linebuf, " pickblock = %i", &buffer)){
      config->pickblock = buffer;
    }
  }

  fclose(file);
}

void config_init()
{
  load_config(&config);
  load_config_key(&keys);
  load_config_mouse(&mouse);
}

