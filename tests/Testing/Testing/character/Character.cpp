#include "Character.hpp"

Character::Character(TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath) {

    Character::sprite = new SpriteDrawingObject(textureAsset, z, vertexShaderPath, fragmentShaderPath);

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
