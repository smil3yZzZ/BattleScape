#pragma once

#ifndef CHARACTER_H
    #define CHARACTER_H

#include "../utils/GLImporter.hpp"
#include "../utils/RapidjsonImporter.hpp"
#include "../texture/Texture.hpp"
#include "../texture/TextureAsset.hpp"

#endif

class Character {

private:
    SpriteDrawingObject* drawingObject;

public:
    Character(int dimension, TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath);
};
