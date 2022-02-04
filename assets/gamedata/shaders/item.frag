#version 440 core
layout (binding = 0) uniform sampler2D atlas;

layout (location = 2) uniform int blockId;

struct light_t
{
    vec3 pos;
    vec3 color;
};

// Grab Player light from UBO
layout(std140, binding = 1) uniform LightsUBO
{
    light_t light[10];
};

layout(std140, binding = 2) uniform FULLBRIGHT
{
    int doFullbright;
};

in vec3 normal  ;
in vec4 fragPos ;
in vec2 texCoord;

out vec4 FragColor;

in vec3 pos;

void main()
{
    vec3 FragPos = vec3(fragPos);

    vec3 diffuse;
    vec3 ambient;

    for(int i = 0; i < 10; i++)
    {
        // Distance from world pixel to light
        float dist = length(vec3(FragPos - light[i].pos));
        float atten = 1.0 / (1.0 + 0.1 * dist + 0.032 * dist * dist);

        // Direction from light to pixel
        vec3 pLightDir = normalize(light[i].pos - FragPos.xyz);

        // Dot product of pixel normal and point light direction
        float diff = max(dot(normal, pLightDir), 0.0) * atten;

        diffuse += vec3(texture(atlas, texCoord)) * diff * vec3(light[i].color);
        if(doFullbright == 1){
            atten = 1.0 ;/// (1.0 + 0.01 * dist + 0.001 * dist * dist);
        }
        else{
            atten = 1.0 / (1.0 + 0.01 * dist + 0.03 * dist * dist);
        }

        ambient += vec3(texture(atlas, texCoord)) * atten  * vec3(light[i].color);
    }

    FragColor = vec4((ambient + diffuse), 1.0);
}