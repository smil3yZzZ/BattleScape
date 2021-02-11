#pragma once
#ifndef TEXTURE_H
    #define TEXTURE_H
#include "utils/TextureAsset.hpp"
#include "utils/GLMImporter.hpp"
#include "utils/GLImporter.hpp"
#endif

class Texture {
private:
    unsigned int id;

public:
    Texture(TextureAsset* textureAsset);
};

//Corregir instrucciones de preprocesado (endif antes de funciones), sustituir TextureAsset por TextureAsset
