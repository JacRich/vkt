#include "window.h"
#include "shader.h"
#include "cmesh.h"
#include "mesh.h"
#include "config.h"
#include "texture.h"
#include "hud.h"
#include "ubo.h"
#include <string>


GLFWwindow *window;

shader_t sh_world, sh_cursor, sh_cross, sh_hud, sh_item;
texture_t tex_atlas;
ubo_t ubo_view_world, ubo_view_hud, ubo_lights, ubo_fullbright;
view_t view_world, view_hud;


static void on_resize(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void window_init()
{
  if (glfwInit() != GLFW_TRUE){
    printf("GLFW failed to init\n");
    exit(0);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

  window = glfwCreateWindow(config.width, config.height, "vkt", NULL, NULL);

  if (window == NULL){
    printf("GLFW failed to create window with code: ");
    int code = glfwGetError(NULL);
    printf("%d\n", code);
    if (code == 65543){
      printf("GLFW_VERSION_UNAVAILABLE: The requested OpenGL or OpenGL ES version (including any requested context or framebuffer hints) is not available on this machine.\n");
    }
    else if (code == 65540){
      printf("GLFW_INVALID_VALUE: One of the arguments to the function was an invalid value, for example requesting a non-existent OpenGL or OpenGL ES version like 2.7.\n");
    }
    exit(0);
  }
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK){
    printf("GLEW failed to init\n");
    exit(0);
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  // Enable Back Face Culling using Counter Clock Wise order
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  // Enable Transparency
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  // Load Shaders
  sh_world = shader_make("gamedata/shaders/world.vert", "gamedata/shaders/world.frag");
  sh_cursor = shader_make("gamedata/shaders/cursor.vert", "gamedata/shaders/cursor.frag");
  sh_cross = shader_make("gamedata/shaders/cross.vert", "gamedata/shaders/cross.frag");
  sh_hud = shader_make("gamedata/shaders/hud.vert", "gamedata/shaders/hud.frag");
  sh_item = shader_make("gamedata/shaders/item.vert", "gamedata/shaders/item.frag");
  // Load Textures
  tex_atlas = texture_make("gamedata/merged_textures.png");

  // Create Uniform Buffer Objects
  ubo_view_world = ubo_make(128, 0);
  ubo_lights = ubo_make(32 * 10, 1); // Oh dear this is bad
  ubo_fullbright = ubo_make(16, 2);
  ubo_view_hud = ubo_make(128, 3);

  // Crosshair
  glLineWidth(2.0f);

  glViewport(0, 0, config.width, config.height);
  glfwSetFramebufferSizeCallback(window, on_resize);
}

void window_tick()
{
  //glViewport(0,0, config.width, config.height);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cmesh_draw();
  static_meshes_tick();

  // Make world view matrices
  view_world.view = glm::lookAt(player.pos, player.pos + player.front, player.up);
  view_world.proj = glm::perspective(glm::radians(float(config.fov)), (float)config.width / (float)config.height,
                                     0.001f, 1000.0f);
  // Make HUD projection matrix
  view_hud.proj = glm::perspective(glm::radians(float(70.0f)), (float)config.width / (float)config.height, 0.001f,
                                   1000.0f);

  ubo_set(ubo_view_hud, &view_hud);
  ubo_set(ubo_view_world, &view_world);
  ubo_set(ubo_fullbright, &config.fullbright);

  glfwSwapBuffers(window);
  // Show FPS in window title
  glfwSetWindowTitle(window, ("FPS: " + std::to_string(1.0 / deltaTime)).c_str());
}

void render_terminate()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}
