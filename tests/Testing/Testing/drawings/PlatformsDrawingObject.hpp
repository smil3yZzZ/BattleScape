#pragma once

#ifndef PLATFORMSDRAWINGOBJECT_H
    #define PLATFORMSDRAWINGOBJECT_H

#include "DrawingObject.hpp"

#endif

class PlatformsDrawingObject : public DrawingObject {
    public:
        PlatformsDrawingObject(int dimension, TextureAsset* textureAsset, float z,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void initVerticesAndIndices(int i, int j, int dimension, int** map);
};
