#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "texture_merge.h"



void create_tex_atlas()
{
  unsigned char *nData = new unsigned char[256 * 256 * 3];

  for(int k = 0; k < 256; k++)
  {
    char name[23];
    sprintf(name, "gamedata/textures/%i", k);
    if(fopen(name, "r") == NULL){
      continue;
    }

    int w, h, n;
    unsigned char *tex = stbi_load(name, &w, &h, &n, 3);

    for (uint8 x = 0; x < 16; x++)
    {
      for (uint8 y = 0; y < 16; y++)
      {
        int curPixel = 3 * ((floor(k / 16) * 16 + y) * 256 + ((k * 16) % 256) + x);
        int texPixel = 3 * (16 * y + x);

        for (uint8 i = 0; i < 3; i++)
        {
          nData[curPixel + i] = tex[texPixel + i];
        }
      }
    }
    stbi_image_free(tex);  
  }

  stbi_write_png("./gamedata/merged_textures.png", 256, 256, 3, nData, 256 * 3);
  delete[] nData;
}