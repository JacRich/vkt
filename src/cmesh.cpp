#include "globaldef.hpp"
#include "cmesh.hpp"
#include "chunk.hpp"

// Vertex Data of a unit cube
const uchar unitcube[6][FACE_SIZE] =
{
// Y+        Normal Block ID
{   1, 1, 1,   0,     0,
    1, 1, 0,   0,     0,
    0, 1, 0,   0,     0,

    0, 1, 0,   0,     0,
    0, 1, 1,   0,     0,
    1, 1, 1,   0,     0,},
// X+                            
{   1, 0, 0,   1,     0,
    1, 1, 0,   1,     0,
    1, 1, 1,   1,     0,

    1, 1, 1,   1,     0,
    1, 0, 1,   1,     0,
    1, 0, 0,   1,     0,},
// Y-                                
{   0, 0, 1,   2,     0,
    0, 0, 0,   2,     0,
    1, 0, 0,   2,     0,

    1, 0, 0,   2,     0,
    1, 0, 1,   2,     0,
    0, 0, 1,   2,     0,},
// X-                                
{   0, 0, 1,   3,     0,
    0, 1, 1,   3,     0,
    0, 1, 0,   3,     0,

    0, 1, 0,   3,     0,
    0, 0, 0,   3,     0,
    0, 0, 1,   3,     0},
// Z+                                
{   1, 0, 1,   4,     0,
    1, 1, 1,   4,     0,
    0, 1, 1,   4,     0,

    0, 1, 1,   4,     0,
    0, 0, 1,   4,     0,
    1, 0, 1,   4,     0},
// Z-                                
{   0, 0, 0,   5,     0,
    0, 1, 0,   5,     0,
    1, 1, 0,   5,     0,

    1, 1, 0,   5,     0,
    1, 0, 0,   5,     0,
    0, 0, 0,   5,     0}
};

int addFace(cmesh_t* mesh, int faceIndex, vec offset, uchar blockID)
{
  int indexOffset = mesh->facecount * FACE_SIZE;

  if ((indexOffset + FACE_SIZE + 1) > BUFFER_SIZE)
  {
    printf("Vertex Buffer exceeded!\n");
    return 0;
  }
  // Set Position data
  for (int i = 0; i < FACE_SIZE; i++)
  {
    mesh->vertdata[i + indexOffset] = unitcube[faceIndex][i];
  }
  // Transform Position
  for (int i = 0; i < FACE_SIZE; i += VERT_SIZE)
  {
    mesh->vertdata[indexOffset + i + 0] += (uchar)offset.x;
    mesh->vertdata[indexOffset + i + 1] += (uchar)offset.y;
    mesh->vertdata[indexOffset + i + 2] += (uchar)offset.z;
  }
  // Set Block ID
  for (int i = 4; i < FACE_SIZE; i += VERT_SIZE)
  {
    mesh->vertdata[indexOffset + i] = blockID;
  }

  mesh->facecount++;
  return 1;
}

void cmesh_build(cmesh_t* mesh)
{          
  for (int i = 0; i < VOXELS_LENGTH; i++)
  {               
    ivec index    = index3d(i, 32);
    vec  voxelPos = ivec_to_vec(index);

    int value = mesh->chunk->voxels[index.x][index.y][index.z];
    // Is voxel air?
    if (value == 0){
      continue;
    }                                                                        

    // Y+
    if(index.y == 31) // Litteral "edge" case amirite haha
    {
      addFace(mesh, 0, voxelPos, value);
    }
    else if(mesh->chunk->voxels[index.x][index.y + 1][index.z] == 0)
    {
      addFace(mesh, 0, voxelPos, value);
    }

    // X+
    if(index.x == 31)
    {
      addFace(mesh, 1, voxelPos, value);
    }
    else if(mesh->chunk->voxels[index.x + 1][index.y][index.z] == 0)
    {
      addFace(mesh, 1, voxelPos, value);
    }

    // Y-
    if(index.y == 0)
    {
      addFace(mesh, 2, voxelPos, value);
    }
    else if(mesh->chunk->voxels[index.x][index.y - 1][index.z] == 0)
    {
      addFace(mesh, 2, voxelPos, value);
    }

    // X-
    if(index.x == 0)
    {
      addFace(mesh, 3, voxelPos, value);
    }
    else if(mesh->chunk->voxels[index.x - 1][index.y][index.z] == 0)
    {
      addFace(mesh, 3, voxelPos, value);
    }

    // Z+
    if(index.z == 31)
    {
      addFace(mesh, 4, voxelPos, value);
    }
    else if(mesh->chunk->voxels[index.x][index.y][index.z + 1] == 0)
    {
      addFace(mesh, 4, voxelPos, value);
    }

    // Z-
    if(index.z == 0)
    {
      addFace(mesh, 5, voxelPos, value);
    }
    else if(mesh->chunk->voxels[index.x][index.y][index.z - 1] == 0)
    {
      addFace(mesh, 5, voxelPos, value);
    }                         
  }
}

void cmesh_send(cmesh_t* mesh)
{
  mesh->vertcount = mesh->facecount * 6;
  glBindVertexArray(mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, sizeof(uchar) * (VERT_SIZE * mesh->vertcount), mesh->vertdata);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mesh->facecount = 0;
}

void cmesh_init(cmesh_t* mesh)
{
  // Allocate memory on the GPU for vertex data
  glGenVertexArrays(1, &mesh->vao);
  glGenBuffers(1, &mesh->vbo);
  glBindVertexArray(mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, (const void*)0, GL_DYNAMIC_DRAW);

  // Mark Vertex Position in buffer  
  const int aPos = 1;
  glVertexAttribIPointer(aPos, 3, GL_UNSIGNED_BYTE, VERT_SIZE, (const void*)0);  
  glEnableVertexAttribArray(aPos);
  // Mark Vertex Normal data in buffer
  const int normalLocation = 2;
  glVertexAttribIPointer(normalLocation, 1, GL_UNSIGNED_BYTE, VERT_SIZE, (const void*)3);
  glEnableVertexAttribArray(normalLocation);
  // Mark Vertex Normal data in buffer
  const int blockIDLocation = 3;  
  glVertexAttribIPointer(blockIDLocation, 1, GL_UNSIGNED_BYTE, VERT_SIZE, (const void*)4);
  glEnableVertexAttribArray(blockIDLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

