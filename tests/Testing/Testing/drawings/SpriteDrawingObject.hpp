#pragma once

#ifndef SPRITEDRAWINGOBJECT_H
    #define SPRITEDRAWINGOBJECT_H

#include "../constants.hpp"
#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"


#endif

class SpriteDrawingObject : public DrawingObject {
    private:
        int direction;
        int sprite;

    public:
        SpriteDrawingObject(TextureAsset* textureAsset,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void updateVerticesAndIndices(float x, float y, int frameState, int direction);
        void update(int newState);
        void changeDirection(int direction);
        void stop();
};
