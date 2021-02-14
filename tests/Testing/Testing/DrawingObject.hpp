#pragma once

#ifndef DRAWINGOBJECT_H
    #define DRAWINGOBJECT_H

#include "utils/RapidjsonImporter.hpp"
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

public:
    DrawingObject(TextureAsset* textureAsset, float z, float* (*initVertices)(int, float*, TextureAsset*, const rapidjson::Document&));
    float* (*initVertices)(int, float*, TextureAsset*, const rapidjson::Document&);
};
