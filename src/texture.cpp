#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void texture_make(texture_t* handle, char const* imagePath) 
{
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, handle);
  glBindTexture(GL_TEXTURE_2D, *handle);

  // Set the texture wrapping and filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  int width, height, nrChannels;
  unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
  if(data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  else {
    printf("Failed to load texture");
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}