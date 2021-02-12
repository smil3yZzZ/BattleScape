#include "TextureAsset.hpp"

TextureAsset::TextureAsset(float textureWidth, float textureHeight, float tileWidth, float tileHeight,
    int numRows, int numCols, int numChannels, unsigned char* data) {
    TextureAsset::textureWidth = textureWidth;
    TextureAsset::textureHeight = textureHeight;
    TextureAsset::tileWidth = tileWidth;
    TextureAsset::tileHeight = tileHeight;
    TextureAsset::numRows = numRows;
    TextureAsset::numCols = numCols;
    TextureAsset::numChannels = numChannels;
    TextureAsset::data = data;
}

float TextureAsset::getTextureWidth() {
    return textureWidth;
}

float TextureAsset::getTextureHeight() {
    return textureHeight;
}

float TextureAsset::getTileWidth() {
    return tileWidth;
}

float TextureAsset::getTileHeight() {
    return tileHeight;
}

int TextureAsset::getNumRows() {
    return numRows;
}

int TextureAsset::getNumCols() {
    return numCols;
}

int TextureAsset::getNumChannels() {
    return numChannels;
}

unsigned char* TextureAsset::getData() {
    return data;
}
