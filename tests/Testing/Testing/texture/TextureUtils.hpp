#pragma once
#ifndef FILELOADER_H
    #define FILELOADER_H
#include "TextureAsset.hpp"
#include "../utils/RapidjsonImporter.hpp"
#include "../drawings/PlatformsDrawingObject.hpp"
#include "../drawings/WallsDrawingObject.hpp"
#endif



class TextureUtils {

public:
	static TextureAsset* loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int verticesPerQuad, int indicesPerQuad, int subsamplingFactor);
    static void initMap(int dimension, int** map, int** wallMap, PlatformsDrawingObject* platforms, WallsDrawingObject* walls, const rapidjson::Document& wallsInfo);
};
