#pragma once
#include "pch.h"
#include "GlAbst/Shader.h"
#include "GlAbst/Texture.h"
#include "GlAbst/VertexPackage.h"
#include "Camera.h"
#include "Material.h"


class Mesh{
    public:
        Mesh(const VertexPackage& vp, Material* Mat) :  Vp(vp), mat(Mat){}
        void Draw(Shader* Program);
        void Editor();
        Material* GetMat() {return mat;}
    private:
        // Material mat = Material(glm::vec3(0.2f, 0.2f, 0.2f), "container2.png", "container2_specular.png", 32.0f);
        Material* mat;
        VertexPackage Vp;
};