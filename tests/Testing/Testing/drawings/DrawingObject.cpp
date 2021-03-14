#include "DrawingObject.hpp"

DrawingObject::DrawingObject(TextureAsset* textureAsset, float z,
                        const char* vertexShaderPath, const char* fragmentShaderPath) {

    DrawingObject::textureAsset = textureAsset;

    DrawingObject::VAO = DrawingObject::VBO = DrawingObject::EBO = 0;

    DrawingObject::z = z;

    DrawingObject::shader = new Shader(vertexShaderPath, fragmentShaderPath);
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

void DrawingObject::initBuffers() {
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);
}

void DrawingObject::updateBuffers(int dimension) {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float vertexBufferSize = textureAsset->getVertexBufferSize();

    int vertices_size = sizeof(float) * dimension * dimension * textureAsset->getVerticesPerQuad() * vertexBufferSize;
    int indices_size = sizeof(unsigned int) * dimension * dimension * textureAsset->getIndicesPerQuad();

    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexBufferSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // gradient attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexBufferSize * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void DrawingObject::render(int dimension, GLfloat* projection, GLfloat* view, GLfloat* model) {
    shader->use();
    shader->setFloatMatrix("projection", projection);
    shader->setFloatMatrix("view", view);
    shader->setFloatMatrix("model", model);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    glDrawElements(GL_TRIANGLES, textureAsset->getIndicesPerQuad() * dimension * dimension, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

DrawingObject::~DrawingObject(){
    delete shader;
    delete texture;
    delete textureAsset;
    delete[] vertices;
    delete[] indices;
}
