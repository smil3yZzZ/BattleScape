#pragma once

#ifndef SHADOWDRAWINGOBJECT_H
#define SHADOWDRAWINGOBJECT_H

#include "../constants.hpp"
#include "DrawingObject.hpp"
#include "../utils/GLImporter.hpp"


#endif

class ShadowDrawingObject : public DrawingObject {
private:

public:
    ShadowDrawingObject(TextureAsset* textureAsset,
        const char* vertexShaderPath, const char* fragmentShaderPath);
    void updateVerticesAndIndices(float x, float y);
};
