#pragma once

#ifndef FIELDOFVIEWDRAWINGOBJECT_H
#define FIELDOFVIEWDRAWINGOBJECT_H

#include "../constants.hpp"
#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"

#endif

class FieldOfViewDrawingObject : public DrawingObject {
private:

public:
    FieldOfViewDrawingObject(TextureAsset* textureAsset,
        const char* vertexShaderPath, const char* fragmentShaderPath);
    void updateVerticesAndIndices(float x, float y);
    void updateBuffers();
};
