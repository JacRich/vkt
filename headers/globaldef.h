#ifndef VKT_DEF_H
#define VKT_DEF_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using uchar = uint8_t ;
using uint  = uint32_t;

using mat4 = glm::mat4;
using vec2 = glm::vec2;
using vec  = glm::vec3;
using vec4 = glm::vec4;

#define VERT_SIZE 5
#define FACE_SIZE 30

#define CHUNK_CROOT   64
#define VOXELS_LENGTH 64 * 64 * 64

#define REGION_CHUNKS_LENGTH 8  // 2^3
#define REGION_CHUNKS_CROOT  2   // Cube root
#define REGION_WIDTH         128 // REGION_CHUNKS_CROOT * CHUNK_CROOT

#define REGION_COUNT        27 // 3^3
#define REGION_COUNT_CROOT  3  // Cube root

#define CMESH_COUNT REGION_CHUNKS_LENGTH * REGION_COUNT // A mesh for each chunk in each region

extern uint sh_world, sh_cursor, sh_cross, sh_hud, sh_item, tex_atlas, tex_item;

extern float lastTime, deltaTime;
extern unsigned long  framecount;
extern float gameTime;

extern GLFWwindow* window;

#endif