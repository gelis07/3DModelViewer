#pragma once
#include "pch.h"

class Shader{
    public:
        Shader(const std::string& vs, const std::string& fs);
        Shader(const std::string& vs, const std::string& gs, const std::string& fs);
        void Bind();
        void Unbind();
        void SetUniform(const std::string& name, int data);
        void SetUniform(const std::string& name, float data);
        void SetUniform(const std::string& name, const glm::vec3 data);
        void SetUniform(const std::string& name, const glm::mat4& mat);
        const GLuint GetID() {return ID;}
    private:
        GLuint ID;

};

