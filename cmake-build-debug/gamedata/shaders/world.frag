#version 440 core
layout (binding = 0) uniform sampler2D atlas;

in vec3 Normal ;
in vec3 FragPos; 
in flat int NormalIndex;
in flat int blockId    ;
in vec2 texCoord;

out vec4 FragColor;

struct light_t
{
	vec3 pos;
	vec3 color;
};

// Grab Player light from UBO
layout(std140, binding = 1) uniform LightsUBO
{	
	light_t light; 
};

layout(std140, binding = 2) uniform FULLBRIGHT
{	
	int doFullbright; 
};

void main()
{		
	vec2 cord = texCoord;
	cord *= 0.0625;
	cord.x += 0.0625 * ((blockId - 1) % 16);
	cord.y += 0.0625 * ((blockId - 1) / 16);

	vec3 diffuse;
	vec3 ambient;

	
  // Distance from world pixel to light
  float dist = length(vec3(FragPos - light.pos));
  float atten = 1.0 / (1.0 + 0.1 * dist + 0.032 * dist * dist);

  // Direction from light to pixel
  vec3 pLightDir = normalize(light.pos - FragPos.xyz); 

  // Dot product of pixel normal and point light direction
  float diff = max(dot(Normal, pLightDir), 0.0) * atten;

  diffuse += vec3(texture(atlas, cord)) * diff * vec3(light.color);
  if(doFullbright == 1){
    atten = 1.0 ;/// (1.0 + 0.01 * dist + 0.001 * dist * dist);
  }
  else{
    atten = 1.0 / (1.0 + 0.01 * dist + 0.03 * dist * dist);
  }
  
  ambient += vec3(texture(atlas, cord)) * atten  * vec3(light.color);  
	
	
	
	FragColor = vec4((ambient + diffuse), 1.0);
}
