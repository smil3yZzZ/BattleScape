#include "SpriteDrawingObject.hpp"

SpriteDrawingObject::SpriteDrawingObject(TextureAsset* textureAsset, const char* vertexShaderPath, const char* fragmentShaderPath):DrawingObject(SPRITE_DRAWING_OBJECT_DIMENSION, textureAsset, CHARACTER_Z_OFFSET, vertexShaderPath, fragmentShaderPath) {
    SpriteDrawingObject::texture = new Texture(textureAsset, GL_LINEAR, GL_NEAREST);
    SpriteDrawingObject::vertices = new float[textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    SpriteDrawingObject::indices = new unsigned int[textureAsset->getIndicesPerQuad()]();
}

void SpriteDrawingObject::initVerticesAndIndices(float initialX, float initialY) {

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();
    float textureWidth = textureAsset->getTextureWidth();

    for (int k = 0; k < verticesPerQuad; k++) {
        int y = k % 2 == 0 ? initialY : initialY + tileHeight;
        vertices[vertexBufferSize * k] = k > 1 ? initialX + tileWidth : initialX;
        vertices[vertexBufferSize * k + 1] = y;
        vertices[vertexBufferSize * k + 2] = zOffset - y;
        vertices[vertexBufferSize * k + 3] = k > 1 ? 0.2f : 0.0f;
        vertices[vertexBufferSize * k + 4] = k % 2 == 0 ? 0.0f : 0.25f;
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
