#include "FogDrawingObject.hpp"

FogDrawingObject::FogDrawingObject(int dimension, TextureAsset* textureAsset, float zOffset,
    const char* vertexShaderPath, const char* fragmentShaderPath) :
    DrawingObject(dimension, textureAsset, zOffset,
        vertexShaderPath, fragmentShaderPath) {
    FogDrawingObject::vertices = new float[dimension * dimension * textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    FogDrawingObject::indices = new unsigned int[dimension * dimension * textureAsset->getIndicesPerQuad()]();
}
 

void FogDrawingObject::initVerticesAndIndices(float xOrigin, float yOrigin) {

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();

    for (int k = 0; k < verticesPerQuad; k++) {
        int vertexY = k % 2 == 0 ? yOrigin : yOrigin + tileHeight;
        vertices[vertexBufferSize * k] = k > 1 ? xOrigin + tileWidth : xOrigin;
        vertices[vertexBufferSize * k + 1] = vertexY;
        vertices[vertexBufferSize * k + 2] = zOffset - vertexY;
     }

    for (int k = 0; k < indicesPerQuad; k++) {
        indices[k] = (k > 2 ? k - 2 : k);
    }
}