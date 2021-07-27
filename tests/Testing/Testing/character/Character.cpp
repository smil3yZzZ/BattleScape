#include "Character.hpp"

Character::Character(TextureAsset* textureAsset,
    const char* vertexShaderPath, const char* fragmentShaderPath) {

    Character::sprite = new SpriteDrawingObject(textureAsset, vertexShaderPath, fragmentShaderPath);

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

void Character::update(int x, int y) {
    sprite->updateVerticesAndIndices(x, y);
}

void Character::move(int newState) {
    if (state == newState) {
        sprite->update(newState);
    }
    else {
        sprite->changeDirection(newState);
    }
}

void Character::stop() {
    sprite->stop();
}

SpriteDrawingObject* Character::getSprite() {
    return sprite;
}
