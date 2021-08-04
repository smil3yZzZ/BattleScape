#pragma once

#ifndef CHARACTER_H
    #define CHARACTER_H

#include "../constants.hpp"
#include "../utils/GLImporter.hpp"
#include "../utils/RapidjsonImporter.hpp"
#include "../texture/Texture.hpp"
#include "../texture/TextureAsset.hpp"
#include "../drawings/SpriteDrawingObject.hpp"
#include "../drawings/ShadowDrawingObject.hpp"


#endif

class Character {

private:
    SpriteDrawingObject* sprite;
    ShadowDrawingObject* shadow;
    float currentX, currentY;
    int frameState;

public:
    Character(TextureAsset* textureAsset, TextureAsset* shadowTextureAsset,
        const char* vertexShaderPath, const char* fragmentShaderPath);
    void update(int x, int y, int frame, int direction);
    int getFrameState(int x, int y, int frame);
    bool collisionExists(int x, int y, int** map);
    SpriteDrawingObject* getSprite();
};
