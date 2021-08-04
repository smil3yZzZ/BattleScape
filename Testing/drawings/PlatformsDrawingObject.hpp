#pragma once

#ifndef PLATFORMSDRAWINGOBJECT_H
    #define PLATFORMSDRAWINGOBJECT_H

#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"

#endif

class PlatformsDrawingObject : public DrawingObject {
    public:
        PlatformsDrawingObject(int dimension, TextureAsset* textureAsset, float zOffset,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void initVerticesAndIndices(int i, int j, int** map);
};
