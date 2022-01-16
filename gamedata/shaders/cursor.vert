#version 440 core
layout (location = 0) uniform mat4 m_model;
layout (location = 1) in ivec3 aPos;


// Grab View Matrices from UBO
layout(std140, row_major, binding = 0) uniform MatricesBlock
{
    mat4 m_view;
	mat4 m_proj;
};

void main()
{
    gl_Position = vec4(aPos, 1.0) * m_model * m_view * m_proj;   
}
