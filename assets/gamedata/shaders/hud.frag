#version 440 core
layout (binding = 0) uniform sampler2D atlas;

layout (location = 2) uniform int blockId;

in vec3 normal  ;
in vec4 fragPos ; 
in vec2 texCoord;

out vec4 FragColor;

// Grab Player light from UBO
layout(std140, binding = 1) uniform Light
{	
	vec4 pos;
	vec4 color;
} light;

in vec3 pos;

void main()
{			
	vec3 dir = max(normalize(vec3(-1, -0.5, 1)), 0);
	vec2 cord = texCoord;
	cord *= 0.0625;
	cord.x += 0.0625 * ((blockId - 1) % 16);
	cord.y += 0.0625 * ((blockId - 1) / 16);
	vec3 diffuse = vec3(texture(atlas, cord) * dot(dir, normal)) * vec3(light.color);
	vec3 ambient = vec3(texture(atlas, cord) * vec4(0.5)) * vec3(light.color);
	
  FragColor = vec4(diffuse + ambient, 1.0);
}