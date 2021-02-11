#include "DrawingObject.hpp"

DrawingObject::DrawingObject(TextureAsset* textureAsset, float z) {

    DrawingObject::texture = new Texture(textureAsset);

    DrawingObject::VAO = DrawingObject::VBO = DrawingObject::EBO = 0;

    DrawingObject::z = z;

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
