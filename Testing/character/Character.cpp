#include "Character.hpp"

Character::Character(TextureAsset* textureAsset, TextureAsset* shadowTextureAsset,
    const char* vertexShaderPath, const char* fragmentShaderPath) {

    Character::sprite = new SpriteDrawingObject(textureAsset, vertexShaderPath, fragmentShaderPath);
    Character::shadow = new ShadowDrawingObject(shadowTextureAsset, vertexShaderPath, fragmentShaderPath);
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

bool Character::collisionExists(int x, int y, int** map) {

    int hypotheticTopRightX = currentX + x + sprite->getTextureAsset()->getTileWidth() - 5.0f;
    int hypotheticTopRightY = currentY + y + sprite->getTextureAsset()->getTileHeight();
    int hypotheticBottomRightX = currentX + x + sprite->getTextureAsset()->getTileWidth() - 5.0f;
    int hypotheticBottomRightY = currentY + y;
    int hypotheticBottomLeftX = currentX + x + 5.0f;
    int hypotheticBottomLeftY = currentY + y;
    int hypotheticTopLeftX = currentX + x + 5.0f;
    int hypotheticTopLeftY = currentY + y + sprite->getTextureAsset()->getTileHeight();

    bool collisionWalls = 
        map[hypotheticBottomLeftY / QUAD_HEIGHT][hypotheticBottomLeftX / QUAD_WIDTH] == 8 ||
        map[hypotheticBottomRightY / QUAD_HEIGHT][hypotheticBottomRightX / QUAD_WIDTH] == 8;

    return collisionWalls;
}

SpriteDrawingObject* Character::getSprite() {
    return sprite;
}
