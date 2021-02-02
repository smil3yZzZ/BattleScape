#pragma once
#ifndef TEXTUREFILE_H
#define TEXTUREFILE_H
#endif

class TextureFile {
private:
	float width;
    float height;
    int nrChannels;
    unsigned char* data;

public:
	TextureFile(float width, float height, int nrChannels, unsigned char* data);
	float getWidth();
};
