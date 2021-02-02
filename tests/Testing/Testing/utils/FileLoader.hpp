#pragma once
#ifndef FILELOADER_H
#define FILELOADER_H
#endif

#include "TextureFile.hpp"

class FileLoader {

public:
	static TextureFile loadTextureFile(const char* path);
};
