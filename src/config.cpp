#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <string.h>

config_t   config;
config_key_t keys;


static void load_config(config_t* config) 
{
  if(access("config", F_OK) != 0){      
    printf("%s\n", "Failed to load config, falling back to default");
    return;
  }

	FILE *file = fopen("config", "r");
  char linebuf[100];

  while(!feof(file)) 
	{
    fgets(linebuf, 100, file);
    // Is line a comment or empty?
    if(linebuf[0] == '#' || strlen(linebuf) < 4){
      continue;
    }

    int editor_collision;
    if(sscanf(linebuf, " editorcoll = %i", &editor_collision)){
      config->editorcoll = editor_collision;  
    }

    int fullbright;
    if (sscanf(linebuf, " fullbright = %i", &fullbright)){ 
      config->fullbright = fullbright; 
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
    printf("%s\n", "Failed to load config, falling back to default");
    return;
  }

	FILE *file = fopen("config_keys", "r");
  char linebuf[100];
  
  while(!feof(file)) 
	{
    fgets(linebuf, 100, file);
    // Is line a comment or empty?
    if (linebuf[0] == '#' || strlen(linebuf) < 4){
      continue;
    }

    int backward;
    if (sscanf(linebuf, " backward = %i", &backward)){
      config->backward = backward;
    }

    int forward;
    if (sscanf(linebuf, " forward = %i", &forward)){
      config->forward = forward;
    }

    int im_blocks;
    if (sscanf(linebuf, " im_blocks = %i", &im_blocks)){
      config->im_blocks = im_blocks;
    }

    int im_range;
    if (sscanf(linebuf, " im_range = %i", &im_range)){
      config->im_range = im_range;
    }

    int im_range_replace;
    if (sscanf(linebuf, " im_range_replace = %i", &im_range_replace)){
      config->im_range_replace = im_range_replace;
    }

    int input_id;
    if (sscanf(linebuf, " input_id = %i", &input_id)){
      config->input_id = input_id;
    }

    int jump;
    if (sscanf(linebuf, " jump = %i", &jump)){
      config->jump = jump;
    }

    int left;
    if (sscanf(linebuf, " left = %i", &left)){
      config->left = left;
    }

    int right;
    if (sscanf(linebuf, " right = %i", &right)){
      config->right = right;
    }

    int toggle_coll;
    if (sscanf(linebuf, " toggle_coll = %i", &toggle_coll)){
      config->toggle_coll = toggle_coll;
    }

    int toggle_cursor;
    if (sscanf(linebuf, " toggle_cursor = %i", &toggle_cursor)){
      config->toggle_cursor = toggle_cursor;
    }

    int toggle_fullbright;
    if (sscanf(linebuf, " toggle_fullbright = %i", &toggle_fullbright)){
      config->toggle_fullbright = toggle_fullbright;
    }

    int toggle_fullscreen;
    if (sscanf(linebuf, " toggle_fullscreen = %i", &toggle_fullscreen)){
      config->toggle_fullscreen = toggle_fullscreen;
    }

    int switchmov;
    if (sscanf(linebuf, " switchmov = %i", &switchmov)){
      config->switchmov = switchmov;
    }
  }

  fclose(file);
}

void config_init()
{
  load_config(&config);
  load_config_key(&keys);
}

