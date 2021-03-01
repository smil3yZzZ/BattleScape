#pragma once
#ifndef FILELOADER_H
    #define FILELOADER_H
#include "TextureAsset.hpp"
#include "RapidjsonImporter.hpp"
#include "../drawings/PlatformsDrawingObject.hpp"
#include "../drawings/WallsDrawingObject.hpp"
#endif



class TextureUtils {

public:
	static TextureAsset loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int verticesPerQuad, int indicesPerQuad);
    static void initMap(int dimension, float** map, float** wallMap, PlatformsDrawingObject* platforms, WallsDrawingObject* walls, const rapidjson::Document& wallsInfo);
};
