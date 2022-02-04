#include "ecs.h"
#include "ubo.h"
#include "perlin.h"
#include "ivec.h"


void lights_tick() {
    uint16_t requiredComps = C_TRANSFORM | C_LIGHT;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].isValid || !entity_hasTags(i, requiredComps)) {
            continue;
        }
        components.lights[i].pos = vec_to_vec4(components.transforms[i].pos);
        // Oh boi this is terrible, I blame opengl
        ubo_set(ubo_lights, &components.lights[i], i * 32);
    }
}
