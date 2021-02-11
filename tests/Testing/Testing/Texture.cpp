#include "Texture.hpp"

Texture::Texture(TextureAsset* textureAsset) {

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureAsset->getWidth(), textureAsset->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureAsset->getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    //initialize texture (pass data)
    //DrawingObject::texture = new Texture();
    //VAO
    //VBO
    //EBO
    //vertices
}
