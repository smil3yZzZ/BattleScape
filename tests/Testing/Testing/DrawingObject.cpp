#include "DrawingObject.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

DrawingObject::DrawingObject(unsigned char* data) {

    DrawingObject::texture = new Texture(data);

    DrawingObject::VAO = DrawingObject::VBO = DrawingObject::EBO = 0;
    //initialize texture (pass data)
    //DrawingObject::texture = new Texture();
    //VAO
    //VBO
    //EBO
    //vertices
}
