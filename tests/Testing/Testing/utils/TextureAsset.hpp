#pragma once
#ifndef TEXTUREASSET_H
    #define TEXTUREASSET_H
#endif

class TextureAsset {
private:
	float width;
    float height;
    int nrChannels;
    unsigned char* data;

public:
	TextureAsset(float width, float height, int nrChannels, unsigned char* data);
	float getWidth();
    float getHeight();
    unsigned char* getData();
};
