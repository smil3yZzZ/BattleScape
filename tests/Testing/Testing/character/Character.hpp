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
    Character(TextureAsset* textureAsset, const char* vertexShaderPath, const char* fragmentShaderPath);
    void update(int x, int y);
    void move(int newState);
    void stop();
    SpriteDrawingObject* getSprite();
};
