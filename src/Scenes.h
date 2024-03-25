#pragma once
#include "pch.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "GlAbst/Framebuffer.h"
class Scene{
    public:
        Scene(int SCREEN_WIDTH, int SCREEN_HEIGHT, int SHADOW_WIDTH, int SHADOW_HEIGHT);
        void CaclulateDepthMap(Shader& program);
        void CalculatePointDepthMap(Shader& program);
        void Render(Shader& program,int width, int height);
        void AddModel(Model&& model); // Refrence by value because the model should be new/copied and not get accesed by the "original" version.
        void AddDirLight(Light&& light);
        void AddPointLight(PointLight&& light);
        void Editor();
        Camera camera;
    private:
        bool NormPerFrag = true;
        std::vector<Model> Models;
        std::vector<Light> DirectionalLights;
        std::vector<PointLight> PointLights;
        void LoadScene(Shader& program); //Basically the raw render code to work for depth testing and the final render.
        glm::mat4 shadowProj; 
        float FarPlane;
        std::vector<glm::mat4> shadowTransforms;
        Framebuffer DepthMapFBO;
        Framebuffer PointDepthMapFBO;
        glm::mat4 Projection;
        glm::mat4 LightProjection;
        glm::mat4 LightSpaceMatrix;
        Camera ShadowCamera;
        Skybox skybox = Skybox({"Skybox/right.jpg","Skybox/left.jpg", "Skybox/top.jpg", "Skybox/bottom.jpg", "Skybox/front.jpg", "Skybox/back.jpg"});

};