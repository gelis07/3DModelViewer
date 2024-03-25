#pragma once
#include "pch.h"
#include "GlAbst/Shader.h"
#include "GlAbst/Framebuffer.h"
const inline static unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
class Light{
    public:
        Light(const glm::vec3& dir, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec) 
        : Direction(dir), Ambient(amb), Diffuse(diff), Specular(spec) {}
        void UI(const std::string& name);
        void SetUniforms(const std::string& name, Shader* program);
        const glm::vec3& GetDir() {return Direction;}
    private:
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Direction;
        glm::vec3 Specular;
};
class PointLight{
    public:
        PointLight(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float consta, float lin, float quad)
        : Pos(pos), Ambient(amb), Diffuse(diff), Specular(spec), constant(consta), linear(lin), quadratic(quad) {}
        void UI(const std::string& name);
        void SetUniforms(const std::string& name, Shader* program);
        const glm::vec3& GetPos() {return Pos;}
    private:
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Pos;
        glm::vec3 Specular;
        float constant, linear, quadratic;
};