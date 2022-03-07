#include "comp_mesh.h"
#include "ecs.h"
#include <vector>


void meshes_tick() 
{
  const uint16 tagsRequired = C_MESH | C_TRANSFORM;

  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (!ent_tags[i].isValid || !entity_hasTags(i, tagsRequired)) {
      continue;
    }
    mesh_t *mesh = &components.meshes[i];

    if (mesh->drawflags & DF_NO_DRAW){
      continue;
    }
    if (mesh->drawflags & DF_NO_DEPTH){
      glDisable(GL_DEPTH_TEST);
    } else {
      glEnable(GL_DEPTH_TEST);
    }
    if(mesh->drawflags & DF_NO_CULL){
      glDisable(GL_CULL_FACE);
    } else {
      glEnable(GL_CULL_FACE);
    }

    glActiveTexture(GL_TEXTURE0); // Why must we do this?
    glBindTexture(GL_TEXTURE_2D, mesh->texture);
    glPolygonMode(GL_FRONT_AND_BACK, mesh->polymode);
    glBindVertexArray(mesh->vao);
    glUseProgram(mesh->shader);
    glUniformMatrix4fv(0, 1, GL_TRUE, &mesh_makematrix(mesh, &components.transforms[i])[0][0]);
    glUniform4fv(1, 1, &mesh->color[0]);
    glUniform1i(2, mesh->customAttrib);
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertcount);
  }
}

mat4 mesh_makematrix(mesh_t *mesh, transform_t *transform) 
{
  mat4 matrix = glm::translate(mat4(1.0f), transform->pos);
  matrix = glm::scale(matrix, transform->scale);
  return matrix;
}

mesh_t mesh_load(char const *path, shader_t shader, texture_t *texture, int drawflags) 
{
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("FAILED TO LOAD MESH @ %s, EXITING\n", path);
    exit(0);
  }

  std::vector<vec> posBuff;
  std::vector<vec2> uvBuff;
  std::vector<vec> normalBuff;
  std::vector<float> vertexData;

  vec buffer_vec;
  vec2 buffer_vec2;

  // Someone plz fix this mess
  char linebuf[100];
  while (!feof(file)) 
  {
    fgets(linebuf, 100, file);
    // Is line a comment or empty?
    if (linebuf[0] == '#' || strlen(linebuf) < 4) {
      continue;
    }
    // Load Vertex Positions
    if (sscanf(linebuf, "v %f%f%f", &buffer_vec.x, &buffer_vec.y, &buffer_vec.z)) {
      posBuff.push_back(buffer_vec);
    }
    // Load Vertex UV's
    if (sscanf(linebuf, "vt %f%f", &buffer_vec2.x, &buffer_vec2.y)) {
      uvBuff.push_back(buffer_vec2);
    }
    // Load Vertex Normals
    if (sscanf(linebuf, "vn %f%f%f", &buffer_vec.x, &buffer_vec.y, &buffer_vec.z)) {
      normalBuff.push_back(buffer_vec);
    }

    // Send the vertex data for each triangle into the buffer, 1 triangle per line
    int indexBuffer[9];
    if (sscanf(linebuf, "f %i/%i/%i %i/%i/%i %i/%i/%i", &indexBuffer[0], &indexBuffer[1], &indexBuffer[2],
              &indexBuffer[3], &indexBuffer[4], &indexBuffer[5], &indexBuffer[6], &indexBuffer[7],
              &indexBuffer[8])) {
      for (int i = 0; i < 9; i += 3) {
        vertexData.push_back(posBuff[indexBuffer[i] - 1].x);
        vertexData.push_back(posBuff[indexBuffer[i] - 1].y);
        vertexData.push_back(posBuff[indexBuffer[i] - 1].z);

        vertexData.push_back(uvBuff[indexBuffer[i + 1] - 1].x);
        vertexData.push_back(uvBuff[indexBuffer[i + 1] - 1].y);

        vertexData.push_back(normalBuff[indexBuffer[i + 2] - 1].x);
        vertexData.push_back(normalBuff[indexBuffer[i + 2] - 1].y);
        vertexData.push_back(normalBuff[indexBuffer[i + 2] - 1].z);
      }
    }
  }

  mesh_t mesh;
  mesh.shader = shader;
  mesh.drawflags = drawflags;
  if (texture != NULL) {
      mesh.texture = *texture;
  }

  mesh.vertcount = vertexData.size() / 8;

  glGenVertexArrays(1, &mesh.vao);
  uint vbo;
  glGenBuffers(1, &vbo);

  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

  // Byte size of one vertex
  int stride = 8 * sizeof(float);

  const int aPos = 2;
  glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, stride, (const void *) (0 * sizeof(float)));
  glEnableVertexAttribArray(aPos);
  const int texCord = 3;
  glVertexAttribPointer(texCord, 2, GL_FLOAT, GL_FALSE, stride, (const void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(texCord);
  const int normalLocation = 4;
  glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void *) (5 * sizeof(float)));
  glEnableVertexAttribArray(normalLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return mesh;
}