#include "TextureUtils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAsset TextureUtils::loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int verticesPerQuad, int indicesPerQuad) {

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	return TextureAsset(width, height, width/((float)numCols), height/((float)numRows), numRows, numCols, nrChannels, vertexBufferSize, verticesPerQuad, indicesPerQuad, data);
}

float* TextureUtils::initPlatformVerticesAndIndices(int dimension, float z, float** map, TextureAsset* textureAsset, const rapidjson::Document& tileData, float* vertices, unsigned int* indices) {

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int color = map[dimension - i - 1][j];

            float top = ((float)color + 1.0f)/(float)textureAsset->getNumRows();
            float bottom = (float)color/(float)textureAsset->getNumRows();

            int vertexBufferSize = textureAsset->getVertexBufferSize();
            int verticesPerQuad = textureAsset->getVerticesPerQuad();
            int indicesPerQuad = textureAsset->getIndicesPerQuad();

            float tileWidth = textureAsset->getTileWidth();
            float tileHeight = textureAsset->getTileHeight();

            for (int k = 0; k < verticesPerQuad; k++) {
                vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k] = k > 1 ? j * tileWidth + tileWidth : j * tileWidth;
                vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 1] = k % 2 == 0 ? i * tileHeight : i * tileHeight + tileHeight;
                vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 2] = z;
                vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 3] = k > 1 ? 1.0f : 0.0f;
                vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 4] = k % 2 == 0 ? bottom : top;
            }

            for (int k = 0; k < indicesPerQuad; k++) {
                indices[i * dimension * indicesPerQuad + indicesPerQuad * j] = i * dimension * verticesPerQuad + verticesPerQuad * j + (k > 2 ? k - 2 : k);
            }

        }
    }
    return vertices;
}

float* TextureUtils::initWallVerticesAndIndices(int dimension, float z, float** wallMap, TextureAsset* textureAsset, const rapidjson::Document& tileData, float* vertices, unsigned int* indices) {
    //rellenar esto
    return nullptr;
}
