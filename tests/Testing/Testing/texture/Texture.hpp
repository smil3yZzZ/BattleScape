#pragma once
#ifndef TEXTURE_H
    #define TEXTURE_H
#include "TextureAsset.hpp"
#include "../utils/GLMImporter.hpp"
#include "../utils/GLImporter.hpp"
#include "../shader/Shader.hpp"
#endif

class Texture {
private:
    unsigned int id;

public:
    Texture(TextureAsset* textureAsset, GLint minFilter, GLint magFilter);
    unsigned int getId();
    ~Texture();
};
