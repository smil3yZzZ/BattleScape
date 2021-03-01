#include "PlatformsDrawingObject.hpp"

PlatformsDrawingObject::PlatformsDrawingObject(int dimension, TextureAsset* textureAsset, float z,
                        const char* vertexShaderPath, const char* fragmentShaderPath):
                        DrawingObject(dimension, textureAsset, z,
                                                vertexShaderPath, fragmentShaderPath) {
}

void PlatformsDrawingObject::initVerticesAndIndices(int i, int j, int dimension, int** map) {
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

PlatformsDrawingObject::~PlatformsDrawingObject(){
    delete shader;
    delete texture;
    delete textureAsset;
    delete[] vertices;
    delete[] indices;
}
