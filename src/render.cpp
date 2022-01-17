#include "render.hpp"
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
#include "cmesh.hpp"
#include "crosshair.hpp"
#include "torch.hpp"
#include <string>
#include "mesh.hpp"
#include "region.hpp"
#include "config.hpp"

#define MAX_MESHES 20
mesh_t meshes[MAX_MESHES]; int meshcount = 0;

GLFWwindow* window;

// From veng
extern cmesh_t cmeshes[];
extern region_t region;
extern config_t config;

// From player
extern player_t player;

extern float lastTime, deltaTime;
extern unsigned long  framecount;

uint sh_world, sh_cursor, sh_cross, sh_hud;
ubo_t ubo_view, ubo_torch;
uint  tex_atlas;

view_t worldview; view_t hudview;
torch_t torch;

void render_addmesh(mesh_t** mesh)
{
  if(meshcount > MAX_MESHES){
    printf("Over Max Meshes!\n");
    return;
  }
  *mesh = &meshes[meshcount];
  meshcount++;
}

static void draw_mesh(mesh_t* mesh)
{
  if(!(mesh->drawflags & DF_VIS)){
    return;
  } 
  if(mesh->drawflags & DF_DEPTH_TEST){
    glEnable(GL_DEPTH_TEST);
  }
  else{
    glDisable(GL_DEPTH_TEST);
  }
  
  glBindVertexArray(mesh->vao);
  glUseProgram  (mesh->shader);
  glUniformMatrix4fv(0, 1, GL_TRUE, &mesh_makematrix(mesh)[0][0]);
  glUniform4fv(1, 1, &mesh->color[0]);
  glUniform1i (2, mesh->customAttrib);
  glDrawArrays(mesh->primtype, 0, mesh->vertcount);
}

static void draw_chunks()
{
  glUseProgram(sh_world);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_atlas);
  glEnable(GL_DEPTH_TEST);

  mat4 m_model;
  for(int i = 0; i < CHUNKS_LENGTH; i++)
  {
    ivec chunkI = index3d(i, 8);
    m_model = glm::translate(mat4(1.0f), region.chunks[chunkI.x][chunkI.y][chunkI.z].pos);
    shader_setMat4(sh_world, "m_model", &m_model[0][0]);
    glBindVertexArray(cmeshes[i].vao);
    glDrawArrays(GL_TRIANGLES, 0, cmeshes[i].vertcount);
  }
}

static void make_texture(uint* handle, char const* imagePath) 
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

static void make_ubo(ubo_t* ubo, uint size, uint binding) 
{
  ubo->size = size;
  glGenBuffers(1, &ubo->handle);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo->handle);
  glBufferData(GL_UNIFORM_BUFFER, ubo->size, (const void*)0, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo->handle);
}

static void ubo_set(ubo_t* ubo, void* data) 
{
  glBindBuffer(GL_UNIFORM_BUFFER, ubo->handle);
  glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)0, ubo->size, data);
}

static void on_resize(GLFWwindow* window, int width, int height) 
{
  glViewport(0,0, width, height);
}

void render_init()
{
  if(glfwInit() != GLFW_TRUE) { 
    printf("GLFW failed to init\n"); 
    exit(0); 
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "vkt", config.fullscreen == true? glfwGetPrimaryMonitor() : NULL, NULL);
  if(window == NULL) { 
    printf("GLFW failed to create window\n");
    exit(0); 
  }
  glfwMakeContextCurrent(window);

  if(glewInit() != GLEW_OK) { 
    printf("GLEW failed to init\n"); 
    exit(0); 
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable   (GL_DEPTH_TEST);
  // Enable Back Face Culling using Counter Clock Wise order
  glEnable   (GL_CULL_FACE ); 
  glFrontFace(GL_CCW );
  glCullFace (GL_BACK);
  // Enable Transparency
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable   (GL_BLEND);
  glLineWidth(2.0f);

  shader_make(&sh_world , "gamedata/shaders/world.vert" , "gamedata/shaders/world.frag" );
  shader_make(&sh_cursor, "gamedata/shaders/cursor.vert", "gamedata/shaders/cursor.frag");
  shader_make(&sh_cross , "gamedata/shaders/cross.vert" , "gamedata/shaders/cross.frag" );
  shader_make(&sh_hud   , "gamedata/shaders/hud.vert"   , "gamedata/shaders/hud.frag"   );

  make_texture(&tex_atlas, "gamedata/las.jpg");
  
  make_ubo(&ubo_view , 128, 0);
  make_ubo(&ubo_torch, 32 * 2, 1);

  glViewport(0,0, config.width, config.height);
  glfwSetFramebufferSizeCallback(window, on_resize);
}

void update_ubos(player_t* player) 
{
  worldview.view = glm::lookAt(player->pos, player->pos + player->front, player->up);
  worldview.proj = glm::perspective(glm::radians(float(config.fov)), (float)config.width / (float)config.height, 0.001f, 1000.0f);

  ubo_set(&ubo_view , &worldview);

  glBindBuffer(GL_UNIFORM_BUFFER, ubo_torch.handle);
  glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)0, sizeof(vec), &torch.pos);
  glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)16, sizeof(vec), &torch.color);
}


void buildmeshes()
{
  for(int i = 0; i < 512; i++)
  {
    if(cmeshes[i].chunk->update == true)
    { 
      cmeshes[i].chunk->update = false;
      cmesh_build(&cmeshes[i], cmeshes[i].chunk);
      return;
    }
  }
}

void render_tick()
{
  //glViewport(0,0, config.width, config.height);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  buildmeshes();

  draw_chunks();
  // Draw static meshes
  for(int i = 0; i < meshcount; i++)
  {
    draw_mesh(&meshes[i]);
  }

  update_ubos(&player);
  glfwSwapBuffers   (window);
  glfwSetWindowTitle(window, ("FPS: " + std::to_string(1.0 / deltaTime)).c_str());
} 

void render_terminate()
{
  glfwDestroyWindow(window);
  glfwTerminate(); 
}


