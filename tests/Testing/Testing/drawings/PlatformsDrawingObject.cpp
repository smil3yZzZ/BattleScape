#include "PlatformsDrawingObject.hpp"

PlatformsDrawingObject::PlatformsDrawingObject(int dimension, TextureAsset* textureAsset, float zOffset,
                        const char* vertexShaderPath, const char* fragmentShaderPath):
                        DrawingObject(dimension, textureAsset, zOffset,
                                                vertexShaderPath, fragmentShaderPath) {
    PlatformsDrawingObject::vertices = new float[dimension * dimension * textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    PlatformsDrawingObject::indices = new unsigned int[dimension * dimension * textureAsset->getIndicesPerQuad()]();
    PlatformsDrawingObject::texture = new Texture(textureAsset, GL_LINEAR, GL_NEAREST);
}

void PlatformsDrawingObject::initVerticesAndIndices(int i, int j, int** map) {

    int color = map[dimension - i - 1][j];

    float top = ((float)color + 1.0f)/(float)textureAsset->getNumRows();
    float bottom = ((float)color)/(float)textureAsset->getNumRows();

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();

    for (int k = 0; k < verticesPerQuad; k++) {
        int y = k % 2 == 0 ? i * tileHeight : i * tileHeight + tileHeight;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k] = k > 1 ? j * tileWidth + tileWidth : j * tileWidth;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 1] = y;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 2] = zOffset - y;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 3] = k > 1 ? 1.0f : 0.0f;
        vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + vertexBufferSize * k + 4] = k % 2 == 0 ? bottom : top;
    }

    std::cout << "PLATFORM:" << std::endl;
    std::cout << vertices[i * dimension * vertexBufferSize * verticesPerQuad + vertexBufferSize * verticesPerQuad * j + 2] << std::endl;

    for (int k = 0; k < indicesPerQuad; k++) {
        indices[i * dimension * indicesPerQuad + indicesPerQuad * j + k] = i * dimension * verticesPerQuad + verticesPerQuad * j + (k > 2 ? k - 2 : k);
    }
}
