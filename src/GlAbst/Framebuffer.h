#pragma once
#include "pch.h"
#include "Texture.h"
class Framebuffer
{
    public:
        Framebuffer(int width, int height, int component, int textureType = GL_TEXTURE_2D);
        void Bind();
        void Unbind();
        const GLuint GetID() {return ID;}
        const Texture& GetTexture() {return texture;}
    private:
        GLuint ID;
        Texture texture;
};