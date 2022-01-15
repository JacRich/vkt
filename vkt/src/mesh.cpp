#include "globaldef.hpp"
#include "mesh.hpp"

mat4 mesh_makematrix(mesh_t* mesh)
{
  mat4 matrix = glm::translate(mat4(1.0f), mesh->pos);
  matrix = glm::scale  (matrix, mesh->scale);
  
  return matrix;
}
