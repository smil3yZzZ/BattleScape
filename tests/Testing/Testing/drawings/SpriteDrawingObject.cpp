#include "SpriteDrawingObject.hpp"

SpriteDrawingObject::SpriteDrawingObject(TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath):DrawingObject(textureAsset, z, vertexShaderPath, fragmentShaderPath) {
    SpriteDrawingObject::texture = new Texture(textureAsset, GL_LINEAR, GL_NEAREST);
    SpriteDrawingObject::vertices = new float[textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    SpriteDrawingObject::indices = new unsigned int[textureAsset->getIndicesPerQuad()]();
}

void SpriteDrawingObject::initVerticesAndIndices(float x, float y) {

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();
    float textureWidth = textureAsset->getTextureWidth();

    for (int k = 0; k < verticesPerQuad; k++) {
        vertices[vertexBufferSize * k] = k > 1 ? x + tileWidth : x;
        vertices[vertexBufferSize * k + 1] = k % 2 == 0 ? y : y + tileHeight;
        vertices[vertexBufferSize * k + 2] = z;
        vertices[vertexBufferSize * k + 3] = k > 1 ? 1.0f : 0.0f;
        vertices[vertexBufferSize * k + 4] = k % 2 == 0 ? 0.0f : 1.0f;
    }

    for (int k = 0; k < indicesPerQuad; k++) {
        indices[k] = (k > 2 ? k - 2 : k);
    }
}

void SpriteDrawingObject::update(int newState) {

}

void SpriteDrawingObject::changeDirection(int direction) {

}

void SpriteDrawingObject::stop() {

}
