#include "FieldOfViewDrawingObject.hpp"

FieldOfViewDrawingObject::FieldOfViewDrawingObject(TextureAsset* textureAsset, const char* vertexShaderPath, const char* fragmentShaderPath) :DrawingObject(SPRITE_DRAWING_OBJECT_DIMENSION, textureAsset, CHARACTER_SHADOW_Z_OFFSET, vertexShaderPath, fragmentShaderPath) {
    FieldOfViewDrawingObject::texture = new Texture(textureAsset, GL_NEAREST, GL_LINEAR);
    FieldOfViewDrawingObject::vertices = new float[textureAsset->getVertexBufferSize() * textureAsset->getVerticesPerQuad()]();
    FieldOfViewDrawingObject::indices = new unsigned int[textureAsset->getIndicesPerQuad()]();
}

void FieldOfViewDrawingObject::updateVerticesAndIndices(float x, float y) {

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
        vertices[vertexBufferSize * k + 3] = 0.0f;
        vertices[vertexBufferSize * k + 4] = 0.0f;
        vertices[vertexBufferSize * k + 5] = 0.0f;
        vertices[vertexBufferSize * k + 6] = 0.0f;
    }

    for (int k = 0; k < indicesPerQuad; k++) {
        indices[k] = (k > 2 ? k - 2 : k);
    }
}

void FieldOfViewDrawingObject::updateBuffers() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float vertexBufferSize = textureAsset->getVertexBufferSize();

    int vertices_size = sizeof(float) * dimension * dimension * textureAsset->getVerticesPerQuad() * vertexBufferSize;
    int indices_size = sizeof(unsigned int) * dimension * dimension * textureAsset->getIndicesPerQuad();

    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexBufferSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexBufferSize * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*

    if (textureAsset->getData() != NULL) {
        // gradient attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexBufferSize * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    */
}
