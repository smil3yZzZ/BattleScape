#pragma once

#ifndef FOGDRAWINGOBJECT_H
    #define FOGDRAWINGOBJECT_H

#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"

#endif

class FogDrawingObject : public DrawingObject {
public:
    FogDrawingObject(int dimension, TextureAsset* textureAsset, float zOffset,
        const char* vertexShaderPath, const char* fragmentShaderPath);
    void initVerticesAndIndices(float xOrigin, float yOrigin);
};