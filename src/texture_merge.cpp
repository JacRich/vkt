#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "texture_merge.h"

void create_tex_atlas() {
    unsigned char *nData = new unsigned char[256 * 256 * 3];

    // deprecated
    // using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    int index = 0;
    for (const auto& dirEntry : std::experimental::filesystem::recursive_directory_iterator("./gamedata/textures/")) {
        int w, h, n;
        unsigned char *tex = stbi_load(dirEntry.path().string().c_str(), &w, &h, &n, 3);

        for (uint8 y = 0; y < 16; y++) {
            for (uint8 x = 0; x < 16; x++) {
                int curPixel = 3 * ((floor(index / 16) * 16 + y) * 256 + ((index * 16) % 256) + x);
                int texPixel = 3 * (16 * y + x);

                for (uint8 i = 0; i < 3; i++) {
                    nData[curPixel + i] = tex[texPixel + i];
                }

            }
        }

        stbi_image_free(tex);
        index++;
    }

    stbi_write_png("./gamedata/merged_textures.png", 256, 256, 3, nData, 256 * 3);
    delete[] nData;
}