#ifndef CONFIG_H
#define CONFIG_H
#include "globaldef.hpp"


struct config_t
{
  bool fullscreen = false;
  int width  = 1280;
  int height = 720 ;
  float fov  = 90.0f;
  float looksens = 0.05f;
};

void load_config(config_t* config);


#endif
