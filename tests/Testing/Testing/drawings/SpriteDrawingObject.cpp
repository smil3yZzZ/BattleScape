#include "SpriteDrawingObject.hpp"

SpriteDrawingObject::SpriteDrawingObject(TextureAsset* textureAsset, const char* vertexShaderPath, const char* fragmentShaderPath):DrawingObject(SPRITE_DRAWING_OBJECT_DIMENSION, textureAsset, CHARACTER_Z_OFFSET, vertexShaderPath, fragmentShaderPath) {
    SpriteDrawingObject::texture = new Texture(textureAsset, GL_NEAREST, GL_LINEAR);
    SpriteDrawingObject::vertices = new float[textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    SpriteDrawingObject::indices = new unsigned int[textureAsset->getIndicesPerQuad()]();
    //Pasar estas variables a "Character.hpp". Mejor separar vista de controlador
    SpriteDrawingObject::currentX = 0.0f;
    SpriteDrawingObject::currentY = 0.0f;
}

void SpriteDrawingObject::updateVerticesAndIndices(float x, float y) {

    currentX += x;
    currentY += y;

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();
    float textureWidth = textureAsset->getTextureWidth();

    for (int k = 0; k < verticesPerQuad; k++) {
        int vertexY = k % 2 == 0 ? currentY + 0.5f: currentY + tileHeight + 0.5f;
        vertices[vertexBufferSize * k] = k > 1 ? currentX + tileWidth + 0.5f: currentX + 0.5f;
        vertices[vertexBufferSize * k + 1] = vertexY;
        vertices[vertexBufferSize * k + 2] = zOffset - vertexY;
        vertices[vertexBufferSize * k + 3] = k > 1 ? 0.2f : 0.0f;
        vertices[vertexBufferSize * k + 4] = k % 2 == 0 ? 0.0f : 0.25f;
    }

    for (int k = 0; k < indicesPerQuad; k++) {
        indices[k] = (k > 2 ? k - 2 : k);
    }
}

void SpriteDrawingObject::update(int frame) {

}

void SpriteDrawingObject::changeDirection(int direction) {

}

void SpriteDrawingObject::stop() {

}
