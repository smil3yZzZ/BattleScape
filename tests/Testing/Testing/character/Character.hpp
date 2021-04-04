#pragma once

#ifndef CHARACTER_H
    #define CHARACTER_H

#include "../constants.hpp"
#include "../utils/GLImporter.hpp"
#include "../utils/RapidjsonImporter.hpp"
#include "../texture/Texture.hpp"
#include "../texture/TextureAsset.hpp"
#include "../drawings/SpriteDrawingObject.hpp"



#endif

class Character {

private:
    int state;
    SpriteDrawingObject* sprite;

public:
    Character(TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath);
    void move(int newState);
    void stop();
};
