#include "Texture.hpp"

Texture::Texture(TextureAsset* textureAsset, GLint minFilter, GLint magFilter) {
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    /*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 0);
    redundancia en brillos de textura para evitar el flickering! --> podríamos sacar los brillos
    aumentar las zonas oscuras de las plataformas (ponerlas más gordas) y quitar los brillos
    */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureAsset->getTextureWidth(), textureAsset->getTextureHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureAsset->getData());
    //glGenerateMipmap(GL_TEXTURE_2D);
}

unsigned int Texture::getId() {
    return id;
}

Texture::~Texture() {
}
