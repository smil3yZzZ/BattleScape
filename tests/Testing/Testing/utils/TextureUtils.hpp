#pragma once
#ifndef FILELOADER_H
    #define FILELOADER_H
#include "TextureAsset.hpp"
#include "RapidjsonImporter.hpp"
#endif



class TextureUtils {

public:
	static TextureAsset loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int numberOfIndices);
    static float* initPlatformVertices(int dimension, float* map, TextureAsset* textureAsset, const rapidjson::Document& tileData);
    static float* initWallVertices(int dimension, float* map, TextureAsset* textureAsset, const rapidjson::Document& tileData);
};
