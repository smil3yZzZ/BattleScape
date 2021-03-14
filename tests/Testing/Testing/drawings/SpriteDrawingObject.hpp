#pragma once

#ifndef SPRITEDRAWINGOBJECT_H
    #define SPRITEDRAWINGOBJECT_H

#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"

#endif

class SpriteDrawingObject : public DrawingObject {
    public:
        SpriteDrawingObject(TextureAsset* textureAsset, float z,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void initVerticesAndIndices(float x, float y, int dimension, int** map);
        void update();
};
