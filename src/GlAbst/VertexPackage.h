#pragma once
#include "pch.h"

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    glm::vec2 TexCoords;
};
class VertexPackage{
    public:
    VertexPackage(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    const GLuint& GetVBO() {return VBO;}
    const GLuint& GetVAO() {return VAO;}
    const GLuint& GetEBO() {return EBO;}
    const unsigned int GetIndCount() {return IndicesCount;}
    void Bind();
    void Unbind();
    private:
        unsigned int IndicesCount;
        GLuint VBO;
        GLuint VAO;
        GLuint EBO;
};