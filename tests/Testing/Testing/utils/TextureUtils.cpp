#include "TextureUtils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAsset TextureUtils::loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int verticesPerQuad, int indicesPerQuad) {

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	return TextureAsset(width, height, width/((float)numCols), height/((float)numRows), numRows, numCols, nrChannels, vertexBufferSize, verticesPerQuad, indicesPerQuad, data);
}

void TextureUtils::initMap(int dimension, int** map, int** wallMap, PlatformsDrawingObject* platforms, WallsDrawingObject* walls, const rapidjson::Document& wallsInfo) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            platforms->initVerticesAndIndices(i, j, dimension, map);
            /* Descomentar tras probar plataformas!
            if (wallMap[dimension - i - 1][j] != -1) {
                walls->initVerticesAndIndices(i, j, dimension, wallMap, wallsInfo);
            }
            */
        }
    }
}
