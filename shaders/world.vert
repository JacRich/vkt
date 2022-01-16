#version 440 core
// Vertex Data Attributes
layout (location = 1) in ivec3 aPos      ;
layout (location = 2) in int   aFaceIndex;
layout (location = 3) in int   aBlockID  ;

layout (location = 0) uniform mat4 m_model;

out vec3 Normal  ;
out vec3  FragPos ;
out vec2 texCoord;
out flat int NormalIndex;
out flat int blockId    ;

// Grab View Matrices from UBO
layout(std140, row_major, binding = 0) uniform MatricesBlock
{
    mat4 m_view;
	mat4 m_proj;
};

void main()
{		
    gl_Position = vec4(aPos, 1.0) * m_model * m_view * m_proj; // Screen Space
    FragPos     = vec3(vec4(aPos, 1.0) * m_model);             // World Space
	blockId     = aBlockID;
	
	NormalIndex = aFaceIndex;
	
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

	switch(aFaceIndex)
	{	    
		// Y+
		case 0:
		Normal = vec3( 0, 1, 0);			
		break;

		// X+
		case 1:
		Normal = vec3(1, 0, 0);			
		break;

		// Y-
		case 2:
		Normal = vec3( 0,-1, 0);			
		break;

		// X-
		case 3:
		Normal = vec3(-1, 0, 0);
		break;

		// Z+
		case 4:
		Normal = vec3(0, 0, 1);
		break;
		
		// Z-
		case 5:
		Normal = vec3( 0, 0,-1);
		break;
	}
}
