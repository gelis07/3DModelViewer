#include "Light.h"

void Light::UI(const std::string& name)
{
    ImGui::Separator();
    ImGui::Text(name.c_str());
    ImGui::DragFloat3(("Diffuse##"+name).c_str(), glm::value_ptr(Diffuse));
    ImGui::DragFloat3(("Ambient##"+name).c_str(), glm::value_ptr(Ambient));
    ImGui::DragFloat3(("Specular##"+name).c_str(), glm::value_ptr(Specular));
    ImGui::DragFloat3(("Position##"+name).c_str(), glm::value_ptr(Direction));
}

void Light::SetUniforms(const std::string& name, Shader *program)
{
    program->SetUniform(name+".diffuse", Diffuse);
    program->SetUniform(name+".specular", Specular);
    program->SetUniform(name+".direction", Direction);
}

void PointLight::UI(const std::string &name)
{
    ImGui::Separator();
    ImGui::Text(name.c_str());
    ImGui::DragFloat3(("Diffuse##"+name).c_str(), glm::value_ptr(Diffuse));
    ImGui::DragFloat3(("Ambient##"+name).c_str(), glm::value_ptr(Ambient));
    ImGui::DragFloat3(("Specular##"+name).c_str(), glm::value_ptr(Specular));
    ImGui::DragFloat3(("Position##"+name).c_str(), glm::value_ptr(Pos));
    ImGui::DragFloat(("Consant##"+name).c_str(), &constant);
    ImGui::DragFloat(("Linear##"+name).c_str(), &linear);
    ImGui::DragFloat(("Quadratic##"+name).c_str(), &quadratic);
}

void PointLight::SetUniforms(const std::string &name, Shader *program)
{
    program->SetUniform(name+".diffuse", Diffuse);
    program->SetUniform(name+".specular", Specular);
    program->SetUniform(name+".ambient", Ambient);
    program->SetUniform(name+".pos", Pos);
    program->SetUniform(name+".constant", constant);
    program->SetUniform(name+".linear", linear);
    program->SetUniform(name+".quadratic", quadratic);
}
