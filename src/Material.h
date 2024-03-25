#pragma once
#include "pch.h"
#include "GlAbst/Shader.h"
#include "GlAbst/VertexPackage.h"
#include "GlAbst/Texture.h"
#include "Camera.h"


class Material{
    public:
        Material(const glm::vec3& amb, const std::vector<Texture>& diff, const std::vector<Texture>& spec, const std::vector<Texture> normal, float shin)
        : Diffuse(diff), Specular(spec), Shininess(shin), Normal(normal) {}
        void SetUniforms(const std::string& name, Shader* program);
        void UI(const std::string& name);
    public:
        inline static glm::vec3 Ambient;
        std::vector<Texture> Diffuse;
        std::vector<Texture> Specular;
        std::vector<Texture> Normal;
        float Shininess;
};

class Skybox{
    public:
        Skybox(const std::vector<std::string>& faces);
        void Draw(const glm::mat4& projection, const Camera& cam);
    private:
        GLuint VAO;
        GLuint VBO;
        Shader Program;
        Texture texture;
};