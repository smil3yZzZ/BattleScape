#pragma once
#ifndef TEXTURE_H
    #define TEXTURE_H
#include "utils/TextureAsset.hpp"
#include "utils/GLMImporter.hpp"
#include "utils/GLImporter.hpp"
#include "utils/Shader.hpp"
#endif

class Texture {
private:
    unsigned int id;

public:
    Texture(TextureAsset* textureAsset);
};
