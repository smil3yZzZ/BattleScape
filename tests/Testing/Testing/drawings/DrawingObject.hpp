#pragma once

#ifndef DRAWINGOBJECT_H
    #define DRAWINGOBJECT_H

#include "../utils/GLImporter.hpp"
#include "../utils/RapidjsonImporter.hpp"
#include "../texture/Texture.hpp"
#include "../texture/TextureAsset.hpp"

#endif

class DrawingObject {
private:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

protected:
    Shader* shader;
    Texture* texture;
    float* vertices;
    unsigned int* indices;
    float zOffset;
    TextureAsset* textureAsset;
    int dimension;

public:
    DrawingObject(int dimension, TextureAsset* textureAsset, float zOffset, const char* vertexShaderPath, const char* fragmentShaderPath);
    void initBuffers();
    void updateBuffers();
    void render(GLfloat* projection, GLfloat* view, GLfloat* model);
    TextureAsset* getTextureAsset();
    ~DrawingObject();
};
