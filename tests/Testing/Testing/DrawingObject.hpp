#pragma once

#ifndef DRAWINGOBJECT_H
    #define DRAWINGOBJECT_H

#include "utils/RapidjsonImporter.hpp"
#include "Texture.hpp"
#include "utils/TextureAsset.hpp"

#endif

class DrawingObject {
private:

    Shader* shader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Texture* texture;

protected:
    float* vertices;
    unsigned int* indices;
    float z;
    TextureAsset* textureAsset;

public:
    DrawingObject(int dimension, TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath);
    void initBuffers();
    void updateBuffers(int dimension);
};
