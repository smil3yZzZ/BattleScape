#include "WallsDrawingObject.hpp"

WallsDrawingObject::WallsDrawingObject(int dimension, TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath):DrawingObject(textureAsset, z, vertexShaderPath, fragmentShaderPath) {
    WallsDrawingObject::vertices = new float[dimension * dimension * textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    WallsDrawingObject::indices = new unsigned int[dimension * dimension * textureAsset->getIndicesPerQuad()]();
    WallsDrawingObject::texture = new Texture(textureAsset, GL_LINEAR, GL_NEAREST);
}

void WallsDrawingObject::initVerticesAndIndices(int i, int j, int dimension, int** map,
                                                const rapidjson::Document& wallsInfo) {

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();
    float textureWidth = textureAsset->getTextureWidth();

    float lowerTileIndexPixel = textureWidth*(wallsInfo[map[dimension - i - 1][j]]["lower"].GetInt()/(textureWidth/tileWidth));
    float upperTileIndexPixel = textureWidth*(wallsInfo[map[dimension - i - 1][j]]["upper"].GetInt()/(textureWidth/tileWidth));

    for (int k = 0; k < verticesPerQuad/2; k++) {
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k] = k > 1 ? j * tileWidth + tileWidth : j * tileWidth;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 1] = k % 2 == 0 ? i * tileHeight * 2 : i * tileHeight * 2 + tileHeight;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 2] = z;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 3] = k > 1 ? (lowerTileIndexPixel + tileWidth)/textureWidth : (lowerTileIndexPixel)/textureWidth;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 4] = k % 2 == 0 ? 0.0f : 1.0f;
    }

    for (int k = 0; k < indicesPerQuad/2; k++) {
        indices[i * dimension * indicesPerQuad + indicesPerQuad * j + k] = i * dimension * verticesPerQuad + verticesPerQuad * j + (k > 2 ? k - 2 : k);
    }

    for (int k = verticesPerQuad/2; k < verticesPerQuad; k++) {
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k] = k > verticesPerQuad/2 + 1 ? j * tileWidth + tileWidth : j * tileWidth;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 1] = k % 2 == 0 ? i * tileHeight * 2 + tileHeight : i * tileHeight * 2 + tileHeight * 2;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 2] = z;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 3] = k > verticesPerQuad/2 + 1 ? (upperTileIndexPixel + tileWidth)/textureWidth : (upperTileIndexPixel)/textureWidth;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 4] = k % 2 == 0 ? 0.0f : 1.0f;
    }

    for (int k = 0; k < indicesPerQuad/2; k++) {
        indices[i * dimension * indicesPerQuad + indicesPerQuad * j + k + indicesPerQuad/2] = i * dimension * verticesPerQuad + verticesPerQuad * j + (k > 2 ? k + verticesPerQuad/2 - 2 : k + verticesPerQuad/2);
    }
}
