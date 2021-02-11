#include "TextureAsset.hpp"

TextureAsset::TextureAsset(float width, float height, int nrChannels, unsigned char* data) {
    TextureAsset::width = width;
    TextureAsset::height = height;
    TextureAsset::nrChannels = nrChannels;
    TextureAsset::data = data;
}

float TextureAsset::getWidth() {
    return width;
}

float TextureAsset::getHeight() {
    return height;
}

unsigned char* TextureAsset::getData() {
    return data;
}
