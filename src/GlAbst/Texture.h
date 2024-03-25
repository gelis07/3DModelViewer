#pragma once
#include "pch.h"
class Texture{
    public:
        Texture(int Type, const std::string& path);
        Texture(int Type, const std::vector<std::string>& path);
        Texture(int Type); // Most likely used for FBO (Frame Buffer Object).
        void Bind() const;
        void Unbind() const;
        void UseSlot(int slot) const;
        
        GLuint& GetID() {return ID;}
        const std::string& GetPath() {return path;}
        int width, height, nrChannels;
        std::string path;
    private:
        void LoadCubeMap(const std::vector<std::string>& faces);
        void LoadImage(const std::string& path);
        GLuint ID;
        GLuint ImageType; // Saves the image type in OpenGL (GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP)
};