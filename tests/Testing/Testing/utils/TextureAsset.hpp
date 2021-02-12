#pragma once
#ifndef TEXTUREASSET_H
    #define TEXTUREASSET_H
#endif

class TextureAsset {
private:
	float textureWidth;
    float textureHeight;
    float tileWidth;
    float tileHeight;
    int numRows;
    int numCols;
    int numChannels;
    unsigned char* data;

public:
	TextureAsset(
        float textureWidth,
        float textureHeight,
        float tileWidth,
        float tileHeight,
        int numRows,
        int numCols,
        int numChannels,
        unsigned char* data
    );
	float getTextureWidth();
    float getTextureHeight();
    float getTileWidth();
    float getTileHeight();
    int getNumRows();
    int getNumCols();
    int getNumChannels();
    unsigned char* getData();
};
