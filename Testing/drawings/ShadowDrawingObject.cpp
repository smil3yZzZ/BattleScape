#include "ShadowDrawingObject.hpp"

ShadowDrawingObject::ShadowDrawingObject(TextureAsset* textureAsset, const char* vertexShaderPath, const char* fragmentShaderPath) :DrawingObject(SPRITE_DRAWING_OBJECT_DIMENSION, textureAsset, CHARACTER_SHADOW_Z_OFFSET, vertexShaderPath, fragmentShaderPath) {
    ShadowDrawingObject::texture = new Texture(textureAsset, GL_NEAREST, GL_LINEAR);
    ShadowDrawingObject::vertices = new float[textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    ShadowDrawingObject::indices = new unsigned int[textureAsset->getIndicesPerQuad()]();
}

void ShadowDrawingObject::updateVerticesAndIndices(float x, float y) {

    int vertexBufferSize = textureAsset->getVertexBufferSize();
    int verticesPerQuad = textureAsset->getVerticesPerQuad();
    int indicesPerQuad = textureAsset->getIndicesPerQuad();

    float tileWidth = textureAsset->getTileWidth();
    float tileHeight = textureAsset->getTileHeight();
    float textureWidth = textureAsset->getTextureWidth();

    for (int k = 0; k < verticesPerQuad; k++) {
        int vertexY = k % 2 == 0 ? y + 0.5f : y + tileHeight + 0.5f;
        vertices[vertexBufferSize * k] = k > 1 ? x + tileWidth + 0.5f : x + 0.5f;
        vertices[vertexBufferSize * k + 1] = vertexY;
        vertices[vertexBufferSize * k + 2] = zOffset - vertexY;
        vertices[vertexBufferSize * k + 3] = k > 1 ? 1.0f : 0.0f;
        vertices[vertexBufferSize * k + 4] = k % 2 == 0 ? 0.0f : 1.0f;
    }

    for (int k = 0; k < indicesPerQuad; k++) {
        indices[k] = (k > 2 ? k - 2 : k);
    }
}
