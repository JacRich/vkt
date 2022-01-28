#include "globaldef.h"
#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <string.h>

mat4 mesh_makematrix(mesh_t* mesh)
{
  mat4 matrix = glm::translate(mat4(1.0f), mesh->pos);
  matrix = glm::scale(matrix, mesh->scale);
  
  return matrix;
}