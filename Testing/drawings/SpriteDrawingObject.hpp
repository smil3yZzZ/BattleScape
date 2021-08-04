#pragma once

#ifndef SPRITEDRAWINGOBJECT_H
    #define SPRITEDRAWINGOBJECT_H

#include "../constants.hpp"
#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"


#endif

class SpriteDrawingObject : public DrawingObject {
    public:
        SpriteDrawingObject(TextureAsset* textureAsset,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void updateVerticesAndIndices(float x, float y, int frameState, int direction);
};
