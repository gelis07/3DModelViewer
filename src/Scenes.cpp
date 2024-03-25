#include "Scenes.h"

Scene::Scene(int SCREEN_WIDTH, int SCREEN_HEIGHT, int SHADOW_WIDTH, int SHADOW_HEIGHT) : DepthMapFBO(SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT)
, PointDepthMapFBO(SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_TEXTURE_CUBE_MAP)
{
    Projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 1000.0f);
    LightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
    FarPlane = 50.0f;
    shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT, 1.0f, FarPlane); 
}

void Scene::CaclulateDepthMap(Shader &program)
{
    glViewport(0, 0, DepthMapFBO.GetTexture().width, DepthMapFBO.GetTexture().height);
    glCullFace(GL_FRONT);
    DepthMapFBO.Bind();
    program.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    const glm::vec3 ShadowCamPos = glm::normalize(-glm::vec3(DirectionalLights[0].GetDir()));
    glm::mat4 lightView = glm::lookAt(ShadowCamPos, 
                                glm::vec3(0.0f), 
                                glm::vec3(0.0f, 1.0f, 0.0f)); 
    ShadowCamera.view = lightView; 
    LightSpaceMatrix = LightProjection * ShadowCamera.view;
    program.SetUniform("lightSpaceMatrix", LightSpaceMatrix);
    LoadScene(program);
    DepthMapFBO.Unbind();
    glCullFace(GL_BACK);
    program.Unbind();
    DepthMapFBO.Unbind();
}

void Scene::CalculatePointDepthMap(Shader &program)
{
    const glm::vec3& lightPos = PointLights[0].GetPos();
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    glViewport(0, 0, DepthMapFBO.GetTexture().width, DepthMapFBO.GetTexture().height);
    PointDepthMapFBO.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    program.Bind();
    program.SetUniform("LightPos", lightPos);
    program.SetUniform("FarPlane", FarPlane);
    for (unsigned int i = 0; i < 6; ++i)
        program.SetUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    LoadScene(program);
    program.Unbind();
    PointDepthMapFBO.Unbind();
    shadowTransforms.clear();
}

void Scene::Render(Shader& program, int width, int height)
{
    glViewport(0, 0, width, height);
    program.Bind();
    program.SetUniform("LightCount", (int)(DirectionalLights.size()));
    program.SetUniform("PointLightCount", (int)(PointLights.size()));
    program.SetUniform("lightSpaceMatrix", LightSpaceMatrix);
    DepthMapFBO.GetTexture().UseSlot(10);
    program.SetUniform("shadowMap", (int)(10));
    program.SetUniform("AviewPos", camera.GetPos());
    program.SetUniform("projection", Projection);
    program.SetUniform("NormPerFrag", NormPerFrag);
    program.SetUniform("view", camera.view);
    PointDepthMapFBO.GetTexture().UseSlot(9);
    program.SetUniform("PointLightShadowMap", 9);
    program.SetUniform("FarPlane", FarPlane);
    for (int i = 0; i < DirectionalLights.size(); i++)
    {
        DirectionalLights[i].SetUniforms("lights["+std::to_string(i)+"]", &program);
    }
    for (int i = 0; i < PointLights.size(); i++)
    {
        PointLights[i].SetUniforms("Pointlights["+std::to_string(i)+"]", &program);
    }
    LoadScene(program);
    skybox.Draw(Projection, camera);
    program.Unbind();
}

void Scene::AddModel(Model &&model)
{
    Models.push_back(model);
}

void Scene::AddDirLight(Light &&light)
{
    DirectionalLights.push_back(light);
}

void Scene::AddPointLight(PointLight &&light)
{
    PointLights.push_back(light);
}

void Scene::Editor()
{
    ImGui::Begin("Editor");
    ImGui::DragFloat3("Camera", glm::value_ptr(camera.GetPos()));
    ImGui::DragFloat3("Ambient", glm::value_ptr(Material::Ambient));
    ImGui::Checkbox("Normal Per Fragment", &NormPerFrag);
    for (int i = 0; i < DirectionalLights.size(); i++)
    {
        DirectionalLights[i].UI("lights["+std::to_string(i)+"]");
    }
    for (int i = 0; i < PointLights.size(); i++)
    {
        PointLights[i].UI("Pointlights["+std::to_string(i)+"]");
    }
    for (auto &&model : Models)
    {
        for (size_t i = 0; i < model.Meshes.size(); i++)
        {
            Material* mat = model.Meshes[i].GetMat();
            ImGui::InputText(("diffuse##"+std::to_string(i)).c_str(), &(mat->Diffuse[0].path));
            if(ImGui::Button(("LoadDiff##"+std::to_string(i)).c_str())){
                mat->Diffuse[0] = Texture(GL_TEXTURE_2D, mat->Diffuse[0].path);
            }
            // ImGui::InputText("normal##"+std::to_string(i), );
            // if(ImGui::Button("LoadNorm##"+std::to_string(i))){
            //     mat->Normal.push_back(Texture(GL_TEXTURE_2D, ));
            // } 
        }
        
    }
    ImGui::End();
}

void Scene::LoadScene(Shader &program)
{
    for (auto &&model : Models)
    {
        model.Draw(&program);
    }
}