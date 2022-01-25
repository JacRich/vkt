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

using uchar = u_int8_t ;
using uint  = u_int32_t;

using mat4 = glm::mat4;
using vec  = glm::vec3;
using vec4 = glm::vec4;

#define VERT_SIZE 5
#define FACE_SIZE 30
#define VOXELS_LENGTH 32 * 32 * 32

#define REGION_CROOT       4   // Cube root
#define REGION_WIDTH       128 // 4 * 32
#define REGION_CHUNK_COUNT 64  // 4^3
#define REGION_COUNT       27

extern uint sh_world, sh_cursor, sh_cross, sh_hud;

#endif