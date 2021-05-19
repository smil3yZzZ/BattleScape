#pragma once

#ifndef WALLSDRAWINGOBJECT_H
    #define WALLSDRAWINGOBJECT_H

#include "DrawingObject.hpp"
#include "../utils/RapidjsonImporter.hpp"
#include "../utils/GLImporter.hpp"

#endif

class WallsDrawingObject : public DrawingObject {

    public:
        WallsDrawingObject(int dimension, TextureAsset* textureAsset, float zOffset,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void initVerticesAndIndices(int i, int j, int** map, const rapidjson::Document& wallsInfo);
};
