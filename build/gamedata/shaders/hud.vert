#version 440 core
layout (location = 0) uniform mat4 m_model;


// Vertex Data Attributes
layout (location = 2) in vec3 aPos   ;
layout (location = 3) in vec3 aNormal;


// Grab View Matrices from UBO
layout(std140, row_major, binding = 2) uniform MatricesBlock
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

	switch(gl_VertexID % 6)
	{
		case(0):
		texCoord = vec2(0, 0);
		break;

		case(1):
		texCoord = vec2(0, 1);
		break;

		case(2):
		texCoord = vec2(1, 1);
		break;

		case(3):
		texCoord = vec2(1, 1);
		break;

		case(4):
		texCoord = vec2(1, 0);
		break;

		case(5):
		texCoord = vec2(0, 0);
		break;	
	}

	
	mat3 normalMatrix = mat3(m_model);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	normal = normalize(aNormal * normalMatrix);

	gl_Position = vec4(aPos, 1.0) * m_model * m_proj; // Screen Space
}
