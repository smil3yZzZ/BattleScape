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

    float* (*initVerticesWrapped)(int, float, float**, TextureAsset*, const rapidjson::Document&, float*, unsigned int*);

public:
    DrawingObject(int dimension, TextureAsset* textureAsset, float z, float* (*initVertices)(int, float, float**, TextureAsset*, const rapidjson::Document&, float*, unsigned int*));
    void initVertices(int dimension, float** map, TextureAsset* textureAsset, const rapidjson::Document& tileData);
};
