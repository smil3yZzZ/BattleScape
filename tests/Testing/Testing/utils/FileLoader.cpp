#include "FileLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAsset FileLoader::loadTextureAsset(const char* path) {

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/wall.png", &width, &height, &nrChannels, 0);

	return TextureAsset(width, height, nrChannels, data);
}
