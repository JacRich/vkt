#include "config.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>

void load_config(config_t* config) 
{
  if(access("config", F_OK) != 0) // Does file exist
  {      
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

    if (linebuf[0] == '#' || strlen(linebuf) < 4) 
		{
      continue;
    }
    if (sscanf(linebuf, " fullscreen = %s", scanbuf)) 
		{
      int fs = strtol(scanbuf, NULL, 10);
      if(fs == 0 || fs == 1)
      {
        config->fullscreen = fs;
      }
    }
    if (sscanf(linebuf, " width = %s", scanbuf))
    {
      config->width = strtol(scanbuf, NULL, 10);
    }
    if (sscanf(linebuf, " height = %s", scanbuf)) 
    {
      config->height = strtol(scanbuf, NULL, 10);
    }
    if (sscanf(linebuf, " fov = %s", scanbuf)) 
    {
      config->fov = strtod(scanbuf, NULL);
    }
  }

  fclose(file);
}
