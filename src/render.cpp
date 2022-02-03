#include "render.h"
#include "shader.h"
#include "cmesh.h"
#include "mesh.h"
#include "config.h"
#include "texture.h"
#include "hud.h"
#include "ubo.h"
#include <string>
extern cmesh_t* cmeshes;


GLFWwindow* window;

#define MAX_MESHES 50
mesh_t meshes[MAX_MESHES]; int mesh_count = 0;

#define MAX_LIGHTS 10
light_t lights[MAX_LIGHTS]; int light_count = 0;

shader_t  sh_world, sh_cursor, sh_cross, sh_hud, sh_item;
texture_t tex_atlas, tex_item;
ubo_t     ubo_view_world, ubo_view_hud, ubo_lights, ubo_fullbright;
view_t    view_world, view_hud;


static void draw_cmeshes()
{
  glUseProgram   (sh_world);
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture  (GL_TEXTURE_2D, tex_atlas);
  glEnable       (GL_DEPTH_TEST);
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

  mat4 m_model;
  for(int i = 0; i < CMESH_COUNT; i++){
    // Janky way to limit render dist
    if(glm::length((cmeshes[i].chunk->pos + vec{32.0f,32.0f,32.0f}) - player.pos) > config.renderdist){
      continue;
    }

    m_model = glm::translate(mat4(1.0f), cmeshes[i].chunk->pos);
    shader_setMat4(sh_world, "m_model", &m_model[0][0]);
    glBindVertexArray(cmeshes[i].vao);
    glDrawArrays(GL_TRIANGLES, 0, cmeshes[i].vertcount);
  }
}

static void on_resize(GLFWwindow* window, int width, int height) 
{
  glViewport(0, 0, width, height);
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
  //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

  window = glfwCreateWindow(config.width, config.height, "vkt", NULL, NULL);

  if(window == NULL) { 
    printf("GLFW failed to create window with code: ");
    int code = glfwGetError(NULL);
    printf("%d\n", code);
    if (code == 65543) {
       printf("GLFW_VERSION_UNAVAILABLE: The requested OpenGL or OpenGL ES version (including any requested context or framebuffer hints) is not available on this machine.\n");
    } else if (code == 65540) {
       printf("GLFW_INVALID_VALUE: One of the arguments to the function was an invalid value, for example requesting a non-existent OpenGL or OpenGL ES version like 2.7.\n");
    }
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
  glEnable   (GL_CULL_FACE); 
  glFrontFace(GL_CCW );
  glCullFace (GL_BACK);
  // Enable Transparency
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable   (GL_BLEND);
  // Load Shaders
  sh_world  = shader_make("gamedata/shaders/world.vert" , "gamedata/shaders/world.frag" );
  sh_cursor = shader_make("gamedata/shaders/cursor.vert", "gamedata/shaders/cursor.frag");
  sh_cross  = shader_make("gamedata/shaders/cross.vert" , "gamedata/shaders/cross.frag" );
  sh_hud    = shader_make("gamedata/shaders/hud.vert"   , "gamedata/shaders/hud.frag"   );
  sh_item   = shader_make("gamedata/shaders/item.vert"   , "gamedata/shaders/item.frag" );
  // Load Textures
  tex_atlas = texture_make("gamedata/las.jpg");
  tex_item = texture_make("gamedata/test.jpg");
  // Create Uniform Buffer Objects
  ubo_view_world = ubo_make(128, 0);
  ubo_lights     = ubo_make(32 * MAX_LIGHTS, 1);
  ubo_fullbright = ubo_make(16, 2);
  ubo_view_hud = ubo_make  (128, 3);

  // Crosshair
  glLineWidth(2.0f);

  glViewport(0,0, config.width, config.height);
  glfwSetFramebufferSizeCallback(window, on_resize);
}

void render_tick()
{
  //glViewport(0,0, config.width, config.height);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  draw_cmeshes();

  meshes_draw();

  // Make world view matrices
  view_world.view = glm::lookAt(player.pos, player.pos + player.front, player.up);
  view_world.proj = glm::perspective(glm::radians(float(config.fov)), (float)config.width / (float)config.height, 0.001f, 1000.0f);
  // Make HUD projection matrix
  view_hud.proj = glm::perspective(glm::radians(float(70.0f)), (float)config.width / (float)config.height, 0.001f, 1000.0f);
  
  ubo_set(ubo_view_hud  , &view_hud);
  ubo_set(ubo_view_world, &view_world);
  ubo_set(ubo_fullbright, &config.fullbright); 
  ubo_set(ubo_lights    , &lights);
  
  glfwSwapBuffers   (window);
  // Show FPS in window title
  //glfwSetWindowTitle(window, ("FPS: " + std::to_string(1.0 / deltaTime)).c_str());
} 

void render_terminate()
{
  glfwDestroyWindow(window);
  glfwTerminate(); 
}


