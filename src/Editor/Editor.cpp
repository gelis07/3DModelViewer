#include "Editor.h"
#include <optional>

std::optional<Model*> SelModel;
std::optional<Light*> SelDirLight;
std::optional<PointLight*> SelPointLight;

void Editor(Scene *scene)
{
    ImGui::Begin("Editor");
    ImGui::DragFloat3("Camera", glm::value_ptr(scene->camera.GetPos()));
    ImGui::DragFloat3("Ambient", glm::value_ptr(Material::Ambient));
    ImGui::Separator();
    if(SelModel != std::nullopt){
        ImGui::DragFloat3("Position", glm::value_ptr(SelModel.value()->Position));
        ImGui::DragFloat3("Scale", glm::value_ptr(SelModel.value()->Scale));
        for (size_t i = 0; i < SelModel.value()->Materials.size(); i++)
        {
            Material* mat = &SelModel.value()->Materials[i];
            ImGui::InputText(("diffuse##"+std::to_string(i)).c_str(), &(mat->Diffuse[0].path));
            if(ImGui::Button(("LoadDiff##"+std::to_string(i)).c_str())){
                mat->Diffuse[0] = Texture(GL_TEXTURE_2D, mat->Diffuse[0].path);
            }
        }
    }else if(SelDirLight != std::nullopt)
    {
        SelDirLight.value()->UI("light");
    }else if(SelPointLight != std::nullopt){
        SelPointLight.value()->UI("Pointlight");
    }
    ImGui::End();
}

void Hierachy(Scene *scene, GLuint VAO, Texture tex, Shader& sh, const glm::mat4& proj)
{
    ImGui::Begin("Hierchy");
    ImGui::SeparatorText("Models:");
    for (int i = 0; i < scene->Models.size(); i++)
    {
        if(ImGui::Selectable(("Model " + std::to_string(i)).c_str())){
            SelDirLight = std::nullopt;
            SelPointLight = std::nullopt;
            SelModel = &scene->Models[i];
        }
    }
    ImGui::SeparatorText("Dir Lights:");
    for (int i = 0; i < scene->DirectionalLights.size(); i++)
    {
        if(ImGui::Selectable(("Dir Light " + std::to_string(i)).c_str())){
            SelDirLight = &scene->DirectionalLights[i];
            SelPointLight = std::nullopt;
            SelModel = std::nullopt;
        }
    }
    ImGui::SeparatorText("Point Lights:");
    for (int i = 0; i < scene->PointLights.size(); i++)
    {
        glBindVertexArray(VAO);
        sh.Bind();
        tex.UseSlot(1);
        sh.SetUniform("tex", (int)1);
        glm::mat4 model(1.0f);
        // glm::vec3 dir = glm::normalize(scene->camera.GetPos() - scene->PointLights[i].GetPos() );
        // model = glm::lookAt(scene->PointLights[i].GetPos(), scene->PointLights[i].GetPos() + dir, glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, scene->PointLights[i].GetPos());
        model = glm::scale(model, glm::vec3(0.3f));
        glm::mat4 mvp = proj * scene->camera.view * model;
        sh.SetUniform("mvp", mvp);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        sh.Unbind();
        glBindVertexArray(0);
        if(ImGui::Selectable(("Point Light " + std::to_string(i)).c_str())){
            SelDirLight = std::nullopt;
            SelPointLight = &scene->PointLights[i];
            SelModel = std::nullopt;
        }
    }
    if(ImGui::Button("Add Point Light")){
        PointLight pLight1(glm::vec3(0.0f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
        scene->AddPointLight(std::move(pLight1));
    }
    if(ImGui::Button("Add Directional light")){
        Light light1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f));
        scene->AddDirLight(std::move(light1));
    }
    ImGui::End();
}

// ImGui::Begin("Editor");
// ImGui::DragFloat3("Camera", glm::value_ptr(scene->camera.GetPos()));
// ImGui::DragFloat3("Ambient", glm::value_ptr(Material::Ambient));
// for (int i = 0; i < scene->DirectionalLights.size(); i++)
// {
//     scene->DirectionalLights[i].UI("lights["+std::to_string(i)+"]");
// }
// for (int i = 0; i < scene->PointLights.size(); i++)
// {
//     scene->PointLights[i].UI("Pointlights["+std::to_string(i)+"]");
// }
// for (auto &&model : scene->Models)
// {
//     for (size_t i = 0; i < model.Materials.size(); i++)
//     {
//         Material* mat = &model.Materials[i];
//         ImGui::InputText(("diffuse##"+std::to_string(i)).c_str(), &(mat->Diffuse[0].path));
//         if(ImGui::Button(("LoadDiff##"+std::to_string(i)).c_str())){
//             mat->Diffuse[0] = Texture(GL_TEXTURE_2D, mat->Diffuse[0].path);
//         }
//     }
// }
// ImGui::End();