#pragma once

#ifndef DRAWINGOBJECT_H
    #define DRAWINGOBJECT_H


#include "Texture.hpp"
#include "utils/TextureAsset.hpp"

#endif

class DrawingObject {
private:
    float* vertices;
    unsigned int* indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    float z;

    Texture* texture;

    int initVertices();

public:
    DrawingObject(TextureAsset* textureAsset, float z);
};
