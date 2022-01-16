#include "shader.hpp"

static char* load_source(char const* path)
{
  char* buffer = 0;
  long length;
  FILE* f = fopen(path, "rb");
  if(f){
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (char*)malloc((length+1)*sizeof(char));
    if(buffer){
      fread(buffer, sizeof(char), length, f);
    }
    fclose(f);
  }
  else{
    printf("FAILED TO LOAD SHADER: %s, EXITING\n", path);
    exit(0);
  }

  buffer[length] = '\0';
  return buffer;
}

static void compile_shader(uint* handle, uint type, char const* source)
{
  *handle = glCreateShader(type);
  glShaderSource(*handle, 1, &source, NULL);
  glCompileShader(*handle);

  int result;
  glGetShaderiv(*handle, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE)
  {
    int length;
    glGetShaderiv(*handle, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(*handle, length, &length, message);
    printf("%s\n", message);
  }
}

void shader_make(uint* handle, char const* vertPath, char const* fragPath)
{
  char const* vertSource = load_source(vertPath);
  char const* fragSource = load_source(fragPath);
  
  *handle = glCreateProgram();
  uint vs, fs;
  compile_shader(&vs, GL_VERTEX_SHADER  , vertSource);
  compile_shader(&fs, GL_FRAGMENT_SHADER, fragSource);
  free((void*)vertSource);
  free((void*)fragSource);

  glAttachShader(*handle, vs);
  glAttachShader(*handle, fs);
  glLinkProgram (*handle);

  glValidateProgram(*handle); // ERROR HANDLING IS FOR PUSS

  glDeleteShader(vs);
  glDeleteShader(fs);
}

void shader_make(uint* handle, char const* vertPath, char const* fragPath, char const* geoPath)
{
  char const* vertSource = load_source(vertPath);
  char const* fragSource = load_source(fragPath);
  char const* geoSource  = load_source(geoPath );

  *handle = glCreateProgram();
  uint vs, gs, fs;
  compile_shader(&vs, GL_VERTEX_SHADER  , vertSource);
  compile_shader(&fs, GL_FRAGMENT_SHADER, fragSource);
  compile_shader(&gs, GL_GEOMETRY_SHADER, geoSource);
  free((void*)vertSource);
  free((void*)fragSource);
  free((void*)geoSource );

  glAttachShader(*handle, vs);
  glAttachShader(*handle, fs);
  glAttachShader(*handle, gs);
  glLinkProgram (*handle);

  glValidateProgram(*handle); // ERROR HANDLING IS FOR PUSS

  glDeleteShader(vs);
  glDeleteShader(gs);
  glDeleteShader(fs);
}

void shader_setInt(uint handle, const char* name, int value)
{
  glUniform1i(glGetUniformLocation(handle, name), value);
}

void shader_setFloat(uint handle, const char* name, float value)
{
  glUniform1f(glGetUniformLocation(handle, name), value); 
}

void shader_setVec3(uint handle, const char* name, vec value)
{
  glUniform3fv(glGetUniformLocation(handle, name), 1, &value[0]); 
}

void shader_setVec4(uint handle, const char* name, vec4 value)
{
  glUniform4fv(glGetUniformLocation(handle, name), 1, &value[0]); 
}

void shader_setMat4(uint handle, const char* name, float* value)
{
  glUniformMatrix4fv(glGetUniformLocation(handle, name), 1, GL_TRUE, value);
}