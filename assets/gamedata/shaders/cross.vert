#version 440 core
layout (location = 2) in vec3 aPos;

// Grab View Matrices from UBO
layout(std140, row_major, binding = 3) uniform MatricesBlock
{
  mat4 m_view;
	mat4 m_proj;
};

void main()
{
  gl_Position = vec4(vec2(aPos), -0.5, 1.0) * m_proj;
}
