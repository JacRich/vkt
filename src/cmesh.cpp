#include "cmesh.h"
#include "shader.h"
#include "player.h"
#include "config.h"
#include <thread>

cmesh_t *cmeshes;
static std::thread thread;
static bool thread_active = true;

// Vertex Data of a unit cube
const uint8 unitcube[6][FACE_SIZE] =
    {
        // Y+        Normal Block ID
        {
            1,
            1,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,

            0,
            1,
            0,
            0,
            0,
            0,
            1,
            1,
            0,
            0,
            1,
            1,
            1,
            0,
            0,
        },
        // X+
        {
            1,
            0,
            0,
            1,
            0,
            1,
            1,
            0,
            1,
            0,
            1,
            1,
            1,
            1,
            0,

            1,
            1,
            1,
            1,
            0,
            1,
            0,
            1,
            1,
            0,
            1,
            0,
            0,
            1,
            0,
        },
        // Y-
        {
            0,
            0,
            1,
            2,
            0,
            0,
            0,
            0,
            2,
            0,
            1,
            0,
            0,
            2,
            0,

            1,
            0,
            0,
            2,
            0,
            1,
            0,
            1,
            2,
            0,
            0,
            0,
            1,
            2,
            0,
        },
        // X-
        {0, 0, 1, 3, 0,
         0, 1, 1, 3, 0,
         0, 1, 0, 3, 0,

         0, 1, 0, 3, 0,
         0, 0, 0, 3, 0,
         0, 0, 1, 3, 0},
        // Z+
        {1, 0, 1, 4, 0,
         1, 1, 1, 4, 0,
         0, 1, 1, 4, 0,

         0, 1, 1, 4, 0,
         0, 0, 1, 4, 0,
         1, 0, 1, 4, 0},
        // Z-
        {0, 0, 0, 5, 0,
         0, 1, 0, 5, 0,
         1, 1, 0, 5, 0,

         1, 1, 0, 5, 0,
         1, 0, 0, 5, 0,
         0, 0, 0, 5, 0}};

static void add_face(cmesh_t *mesh, int faceIndex, vec offset, uint8 blockID)
{
  int indexOffset = mesh->facecount * FACE_SIZE;

  if ((indexOffset + FACE_SIZE + 1) > BUFFER_SIZE)
  {
    printf("Vertex Buffer exceeded!\n");
    return;
  }
  // Set Position data
  for (int i = 0; i < FACE_SIZE; i++)
  {
    mesh->vertdata[i + indexOffset] = unitcube[faceIndex][i];
  }
  // Transform Position
  for (int i = 0; i < FACE_SIZE; i += VERT_SIZE)
  {
    mesh->vertdata[indexOffset + i + 0] += (uint8)offset.x;
    mesh->vertdata[indexOffset + i + 1] += (uint8)offset.y;
    mesh->vertdata[indexOffset + i + 2] += (uint8)offset.z;
  }
  // Set Block ID
  for (int i = 4; i < FACE_SIZE; i += VERT_SIZE)
  {
    mesh->vertdata[indexOffset + i] = blockID;
  }

  mesh->facecount++;
}

static void cmesh_build(cmesh_t *mesh)
{
  for (int i = 0; i < VOXELS_LENGTH; i++)
  {
    ivec index = index3d(i, CHUNK_CROOT);
    vec voxelPos = ivec_to_vec(index);

    int value = mesh->chunk->voxels[index.x][index.y][index.z];
    // Is voxel air?
    if (value == 0)
    {
      continue;
    }

    // Y+
    if (index.y == (CHUNK_CROOT - 1))
    {
      add_face(mesh, 0, voxelPos, value);
    }
    else if (mesh->chunk->voxels[index.x][index.y + 1][index.z] == 0)
    {
      add_face(mesh, 0, voxelPos, value);
    }

    // X+
    if (index.x == (CHUNK_CROOT - 1))
    {
      add_face(mesh, 1, voxelPos, value);
    }
    else if (mesh->chunk->voxels[index.x + 1][index.y][index.z] == 0)
    {
      add_face(mesh, 1, voxelPos, value);
    }

    // Y-
    if (index.y == 0)
    {
      add_face(mesh, 2, voxelPos, value);
    }
    else if (mesh->chunk->voxels[index.x][index.y - 1][index.z] == 0)
    {
      add_face(mesh, 2, voxelPos, value);
    }

    // X-
    if (index.x == 0)
    {
      add_face(mesh, 3, voxelPos, value);
    }
    else if (mesh->chunk->voxels[index.x - 1][index.y][index.z] == 0)
    {
      add_face(mesh, 3, voxelPos, value);
    }

    // Z+
    if (index.z == (CHUNK_CROOT - 1))
    {
      add_face(mesh, 4, voxelPos, value);
    }
    else if (mesh->chunk->voxels[index.x][index.y][index.z + 1] == 0)
    {
      add_face(mesh, 4, voxelPos, value);
    }

    // Z-
    if (index.z == 0)
    {
      add_face(mesh, 5, voxelPos, value);
    }
    else if (mesh->chunk->voxels[index.x][index.y][index.z - 1] == 0)
    {
      add_face(mesh, 5, voxelPos, value);
    }
  }
}

static void cmesh_send(cmesh_t *mesh)
{
  mesh->vertcount = mesh->facecount * 6;
  glBindVertexArray(mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, sizeof(uint8) * (VERT_SIZE * mesh->vertcount), mesh->vertdata);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mesh->facecount = 0;
}

static void cmesh_init(cmesh_t *mesh)
{
  // Allocate memory on the GPU for vertex data
  glGenVertexArrays(1, &mesh->vao);
  glGenBuffers(1, &mesh->vbo);
  glBindVertexArray(mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, (const void *)0, GL_DYNAMIC_DRAW);

  // Mark Vertex Position in buffer
  const int aPos = 1;
  glVertexAttribIPointer(aPos, 3, GL_UNSIGNED_BYTE, VERT_SIZE, (const void *)0);
  glEnableVertexAttribArray(aPos);
  // Mark Vertex Normal data in buffer
  const int normalLocation = 2;
  glVertexAttribIPointer(normalLocation, 1, GL_UNSIGNED_BYTE, VERT_SIZE, (const void *)3);
  glEnableVertexAttribArray(normalLocation);
  // Mark Vertex Normal data in buffer
  const int blockIDLocation = 3;
  glVertexAttribIPointer(blockIDLocation, 1, GL_UNSIGNED_BYTE, VERT_SIZE, (const void *)4);
  glEnableVertexAttribArray(blockIDLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void cmesh_draw()
{
  glUseProgram(sh_world);
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_atlas);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  mat4 m_model;
  for (int i = 0; i < CMESH_COUNT; i++)
  {
    // Janky way to limit render dist
    if (glm::length((cmeshes[i].chunk->pos + vec{32.0f, 32.0f, 32.0f}) - player.pos) > config.renderdist){
      continue;
    }

    m_model = glm::translate(mat4(1.0f), cmeshes[i].chunk->pos);
    shader_setMat4(sh_world, "m_model", &m_model[0][0]);
    glBindVertexArray(cmeshes[i].vao);
    glDrawArrays(GL_TRIANGLES, 0, cmeshes[i].vertcount);
  }
}

void cmeshes_tick()
{
  for (int i = 0; i < CMESH_COUNT; i++)
  {
    if (thread_active)
    {
      break;
    }
    if (cmeshes[i].needsend == true)
    {
      cmesh_send(&cmeshes[i]);
      cmeshes[i].needsend = false;
      thread_active = true;
      return;
    }
  }
}

static void thread_build_cmeshes()
{
  while (!glfwWindowShouldClose(window))
  {
    if (!thread_active)
    {
      continue;
    }
    for (int i = 0; i < CMESH_COUNT; i++)
    {
      if (cmeshes[i].chunk->update == true && !cmeshes[i].needsend)
      {
        cmeshes[i].needsend = true;
        cmesh_build(&cmeshes[i]);

        cmeshes[i].chunk->update = false;
        thread_active = false;
        break;
      }
    }
  }
}

void meshing_init(region_t regions[REGION_COUNT])
{
  // Init chunk meshes
  cmeshes = (cmesh_t *)malloc(sizeof(cmesh_t) * CMESH_COUNT);
  for (int i = 0; i < CMESH_COUNT; i++)
  {
    cmesh_init(&cmeshes[i]);
  }

  int mesh = 0;
  for (int i = 0; i < REGION_COUNT; i++)
  {
    for (int j = 0; j < REGION_CHUNKS_LENGTH; j++)
    {
      ivec chunkIndex = index3d(j, REGION_CHUNKS_CROOT);
      cmeshes[mesh].chunk = &regions[i].chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z];
      cmeshes[mesh].chunk->update = true;
      mesh++;
    }
  }

  thread = std::thread(thread_build_cmeshes);
}

void meshing_terminate()
{
  thread.join();
  thread.~thread();

  free(cmeshes);
}
