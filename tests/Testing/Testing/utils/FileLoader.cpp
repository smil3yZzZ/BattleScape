#include "FileLoader.hpp"

TextureFile FileLoader::loadTextureFile(const char* path) {

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/wall.png", &width, &height, &nrChannels, 0);

	return new TextureFile(width, height, nrChannels, data);
}
