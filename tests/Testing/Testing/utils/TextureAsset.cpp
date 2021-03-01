#include "TextureAsset.hpp"

TextureAsset::TextureAsset(float textureWidth, float textureHeight, float tileWidth, float tileHeight,
    int numRows, int numCols, int numChannels, int vertexBufferSize, int verticesPerQuad,
    int indicesPerQuad, unsigned char* data) {
    TextureAsset::textureWidth = textureWidth;
    TextureAsset::textureHeight = textureHeight;
    TextureAsset::tileWidth = tileWidth;
    TextureAsset::tileHeight = tileHeight;
    TextureAsset::numRows = numRows;
    TextureAsset::numCols = numCols;
    TextureAsset::numChannels = numChannels;
    TextureAsset::vertexBufferSize = vertexBufferSize;
    TextureAsset::verticesPerQuad = verticesPerQuad;
    TextureAsset::indicesPerQuad = indicesPerQuad;
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

int TextureAsset::getVertexBufferSize() {
    return vertexBufferSize;
}

int TextureAsset::getVerticesPerQuad() {
    return verticesPerQuad;
}

int TextureAsset::getIndicesPerQuad() {
    return indicesPerQuad;
}

unsigned char* TextureAsset::getData() {
    return data;
}

TextureAsset::~TextureAsset() {
    
}
