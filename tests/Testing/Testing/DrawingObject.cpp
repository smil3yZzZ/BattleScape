#include "DrawingObject.hpp"

DrawingObject::DrawingObject(int dimension, TextureAsset* textureAsset, float z, float* (*initVertices)(int, float, float**, TextureAsset*, const rapidjson::Document&, float*, unsigned int*),
                        const char* vertexShaderPath, const char* fragmentShaderPath) {

    DrawingObject::texture = new Texture(textureAsset);

    DrawingObject::vertices = new float[dimension * dimension * textureAsset->getVerticesPerQuad()];

    DrawingObject::indices = new unsigned int[dimension * dimension * textureAsset->getIndicesPerQuad()];

    DrawingObject::initVerticesWrapped = initVertices;

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

void DrawingObject::initVerticesAndIndices(int dimension, float** map, TextureAsset* textureAsset, const rapidjson::Document& tileData) {
    this->initVerticesWrapped(dimension, z, map, textureAsset, tileData, vertices, indices);
}
