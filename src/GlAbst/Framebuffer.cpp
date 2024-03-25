#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height, int component, int textureType) : texture(textureType)
{
    glGenFramebuffers(1, &ID);
    texture.Bind();
    if(textureType == GL_TEXTURE_CUBE_MAP)
    {
        for (size_t i = 0; i < 6; i++)
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 
    }else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, component, GL_FLOAT, NULL);
    }
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    if(textureType == GL_TEXTURE_CUBE_MAP){
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture.GetID(), 0);
    }else{
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.GetID(), 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    texture.width = width;
    texture.height = height;
    texture.Unbind(); 
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
