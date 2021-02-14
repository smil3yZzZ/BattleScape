#include "TextureUtils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAsset TextureUtils::loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int numberOfIndices) {

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	return TextureAsset(width, height, width/((float)numCols), height/((float)numRows), numRows, numCols, nrChannels, vertexBufferSize, numberOfIndices, data);
}

float* TextureUtils::initPlatformVertices(int dimension, float* map, TextureAsset* textureAsset, const rapidjson::Document& tileData) {
    //rellenar
    return nullptr;
}

float* TextureUtils::initWallVertices(int dimension, float* wallMap, TextureAsset* textureAsset, const rapidjson::Document& tileData) {
    //rellenar esto
    return nullptr;
}
