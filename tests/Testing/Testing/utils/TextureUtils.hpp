#pragma once
#ifndef FILELOADER_H
    #define FILELOADER_H
#include "TextureAsset.hpp"
#include "RapidjsonImporter.hpp"
#endif



class TextureUtils {

public:
	static TextureAsset loadTextureAsset(const char* path, int numRows, int numCols, int vertexBufferSize, int verticesPerQuad, int indicesPerQuad);
    static float* initPlatformVerticesAndIndices(int dimension, float z, float** map, TextureAsset* textureAsset, const rapidjson::Document& tileData, float* vertices, unsigned int* indices);
    static float* initWallVerticesAndIndices(int dimension, float z, float** map, TextureAsset* textureAsset, const rapidjson::Document& tileData, float* vertices, unsigned int* indices);
};
