#pragma once

#ifndef WALLSDRAWINGOBJECT_H
    #define WALLSDRAWINGOBJECT_H

#include "../DrawingObject.hpp"
#include "../utils/RapidjsonImporter.hpp"

#endif

class WallsDrawingObject : public DrawingObject {
    public:
        WallsDrawingObject(int dimension, TextureAsset* textureAsset, float z,
                                const char* vertexShaderPath, const char* fragmentShaderPath);
        void initVerticesAndIndices(int i, int j, int dimension, float** map, const rapidjson::Document& wallsInfo);
};
