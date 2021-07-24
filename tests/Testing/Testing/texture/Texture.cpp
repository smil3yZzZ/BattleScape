#include "Texture.hpp"

Texture::Texture(TextureAsset* textureAsset, GLint minFilter, GLint magFilter) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureAsset->getTextureWidth(), textureAsset->getTextureHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureAsset->getData());
}

unsigned int Texture::getId() {
    return id;
}

Texture::~Texture() {
}
