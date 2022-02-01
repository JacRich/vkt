#version 440 core
layout (location = 0) uniform mat4 m_model;


// Vertex Data Attributes
layout (location = 2) in vec3 aPos   ;
layout (location = 3) in vec2 aTexCord;
layout (location = 4) in vec3 aNormal;


// Grab View Matrices from UBO
layout(std140, row_major, binding = 0) uniform MatricesBlock
{
  mat4 m_view;
	mat4 m_proj;
};

out vec3 normal  ;
out vec4 fragPos ;
out vec2 texCoord;

out vec3 pos;


void main()
{		  
	pos = aPos;
  fragPos = vec4(aPos, 1.0) * m_model; // World Space	

  texCoord = aTexCord;
	
	mat3 normalMatrix = mat3(m_model);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	normal = normalize(aNormal * normalMatrix);

	gl_Position = vec4(aPos, 1.0) * m_model * m_view * m_proj; // Screen Space
}
