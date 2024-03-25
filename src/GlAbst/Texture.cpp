#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"

Texture::Texture(int Type, const std::string &path) : ImageType(Type)
{
    glGenTextures(1, &ID);
    LoadImage(path);
}

Texture::Texture(int Type, const std::vector<std::string> &path) : ImageType(Type)
{
    glGenTextures(1, &ID);
    LoadCubeMap(path);
}

Texture::Texture(int Type) : ImageType(Type)
{
    glGenTextures(1, &ID);
}

void Texture::Bind() const
{
    glBindTexture(ImageType, ID);
}

void Texture::Unbind() const
{
    glBindTexture(ImageType, 0);
}

void Texture::UseSlot(int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    Bind();
}

void Texture::LoadCubeMap(const std::vector<std::string> &faces)
{
    stbi_set_flip_vertically_on_load(false);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::LoadImage(const std::string &path)
{
    stbi_set_flip_vertically_on_load(false);
    this->path = path;
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
