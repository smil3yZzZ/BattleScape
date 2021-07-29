#include "Character.hpp"

Character::Character(TextureAsset* textureAsset,
    const char* vertexShaderPath, const char* fragmentShaderPath) {

    Character::sprite = new SpriteDrawingObject(textureAsset, vertexShaderPath, fragmentShaderPath);
    Character::currentX = 0.0f;
    Character::currentY = 0.0f;
    Character::frameState = 0;

    //Character::state = DOWN;
    //Init Maze:
    //Pasar como parámetro variables para indexar en array, así como QUAD_HEIGHT y QUAD_WIDTH.
    //Explorar método para indexar textureTileMap de forma común

    //initialize texture (pass data)
    //DrawingObject::texture = new Texture();
    //VAO
    //VBO
    //EBO
    //vertices
}

void Character::update(int x, int y, int frame, int direction) {
    currentX += x;
    currentY += y;

    if (frame % 15 == 0) {
        frameState = getFrameState(x, y, frame);
    }

    sprite->updateVerticesAndIndices(currentX, currentY, frameState, direction);
}

int Character::getFrameState(int x, int y, int frame) {
    if (x == 0 && y == 0) {
        return 0;
    }
    else {
        return frame / 15;
    }
}

SpriteDrawingObject* Character::getSprite() {
    return sprite;
}
