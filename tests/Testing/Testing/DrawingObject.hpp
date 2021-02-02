#include "Texture.hpp"
#include "utils/TextureFile.hpp"

class DrawingObject {
private:
    float* vertices;
    unsigned int* indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    float z;

    Texture texture;

    int initTextures();

public:
    DrawingObject(TextureFile textureFile);
}
