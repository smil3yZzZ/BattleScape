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
    float z;
    TextureAsset* textureAsset;

public:
    DrawingObject(int dimension, TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath);
    void initBuffers();
    void updateBuffers(int dimension);
    void render(int dimension, GLfloat* projection, GLfloat* view, GLfloat* model);
    ~DrawingObject();
};
