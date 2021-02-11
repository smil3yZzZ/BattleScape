#pragma once
#ifndef FILELOADER_H
    #define FILELOADER_H
#include "TextureAsset.hpp"
#endif



class FileLoader {

public:
	static TextureAsset loadTextureAsset(const char* path);
};
