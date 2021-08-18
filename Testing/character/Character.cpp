#include "Character.hpp"

Character::Character(TextureAsset* textureAsset, TextureAsset* shadowTextureAsset,
    const char* vertexShaderPath, const char* fragmentShaderPath, const char* shadowFragmentShaderPath) {

    Character::sprite = new SpriteDrawingObject(textureAsset, vertexShaderPath, fragmentShaderPath);
    Character::shadow = new ShadowDrawingObject(shadowTextureAsset, vertexShaderPath, shadowFragmentShaderPath);
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
    shadow->updateVerticesAndIndices(currentX, currentY - shadow->getTextureAsset()->getTileHeight());
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

void Character::initBuffers() {
    sprite->initBuffers();
    shadow->initBuffers();
}

void Character::updateBuffers() {
    sprite->updateBuffers();
    shadow->updateBuffers();
}

void Character::render(GLfloat* projection, GLfloat* view, GLfloat* model) {
    sprite->render(projection, view, model);
    shadow->render(projection, view, model);
}
