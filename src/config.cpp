#include "config.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <string.h>

config_t   config;
config_key_t keys;


void load_config(config_t* config) 
{
  if(access("config", F_OK) != 0){      
    printf("%s\n", "Failed to load config, falling back to default");
    return;
  }

	FILE *file = fopen("config", "r");
  char linebuf[100];
	char scanbuf[100];

  // Is there a beter way to do this? Probably
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

    if (sscanf(linebuf, " fullbright = %s", scanbuf)){
      int fb = strtol(scanbuf, NULL, 10);
      if(fb == 0 || fb == 1){
        config->fullbright = fb;
      }
    }

    if (sscanf(linebuf, " width = %s", scanbuf)){
      config->width = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " height = %s", scanbuf)){
      config->height = strtol(scanbuf, NULL, 10);
    }
    
    float fov = 12;
    if (sscanf(linebuf, " fov = %f", &fov)){
      config->fov = fov;
    }

    if (sscanf(linebuf, " movespeed = %s", scanbuf)){
      config->moveSpeed = strtod(scanbuf, NULL);
    }

    if (sscanf(linebuf, " friction = %s", scanbuf)){
      config->friction = strtod(scanbuf, NULL);
    }

    if (sscanf(linebuf, " renderdist = %s", scanbuf)){
      config->renderdist = strtod(scanbuf, NULL);
    }

    if (sscanf(linebuf, " jumpforce = %s", scanbuf)){
      config->jumpForce = strtod(scanbuf, NULL);
    }

    if (sscanf(linebuf, " looksens = %s", scanbuf)){
      config->looksens = strtod(scanbuf, NULL);
    }
  }

  fclose(file);
}

void load_config_key(config_key_t* config)
{
  if(access("config", F_OK) != 0){      
    printf("%s\n", "Failed to load config, falling back to default");
    return;
  }

	FILE *file = fopen("config_keys", "r");
  char linebuf[100];
	char scanbuf[100];

  // Is there a beter way to do this? Probably
  while(!feof(file)) 
	{
    fgets(linebuf, 100, file);

    // Is line a comment or empty?
    if (linebuf[0] == '#' || strlen(linebuf) < 4){
      continue;
    }

    if (sscanf(linebuf, " backward = %s", scanbuf)){
      config->backward = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " forward = %s", scanbuf)){
      config->forward = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " im_blocks = %s", scanbuf)){
      config->im_blocks = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " im_range = %s", scanbuf)){
      config->im_range = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " im_range_replace = %s", scanbuf)){
      config->im_range_replace = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " input_id = %s", scanbuf)){
      config->input_id = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " jump = %s", scanbuf)){
      config->jump = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " left = %s", scanbuf)){
      config->left = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " right = %s", scanbuf)){
      config->right = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " toggle_coll = %s", scanbuf)){
      config->toggle_coll = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " toggle_cursor = %s", scanbuf)){
      config->toggle_cursor = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " toggle_fullbright = %s", scanbuf)){
      config->toggle_fullbright = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " toggle_fullscreen = %s", scanbuf)){
      config->toggle_fullscreen = strtol(scanbuf, NULL, 10);
    }

    if (sscanf(linebuf, " switchmov = %s", scanbuf)){
      config->switchmov = strtol(scanbuf, NULL, 10);
    }
  }

  fclose(file);
}

void config_init()
{
  load_config(&config);
  load_config_key(&keys);
}

