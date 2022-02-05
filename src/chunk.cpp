#include "chunk.h"
#include "perlin.h"
#include "ivec.h"

void chunk_fill_perlin(chunk_t *chunk) {
    for (int i = 0; i < VOXELS_LENGTH; i++) {
        ivec index = index3d(i, CHUNK_CROOT);

        const double samplingScale = 38;
        double perlinSample = perlin_3d((chunk->pos.x + index.x + 500) / samplingScale,
                                        (chunk->pos.y + index.y + 500) / samplingScale,
                                        (chunk->pos.z + index.z + 500) / samplingScale);

        if (perlinSample > 0.40) {
            chunk->voxels[index.x][index.y][index.z] = 0;
        } else {
            chunk->voxels[index.x][index.y][index.z] = 5;
        }
    }
}

void chunk_fill(chunk_t *chunk, uint8 value) {
    for (int i = 0; i < VOXELS_LENGTH; i++) {
        ivec index = index3d(i, CHUNK_CROOT);

        chunk->voxels[index.x][index.y][index.z] = value;
    }
}
