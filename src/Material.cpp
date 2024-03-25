#include "Material.h"
#include "stb_image.h"

void Material::SetUniforms(const std::string &name, Shader *program)
{
    // program->SetUniform("tex", Diffuse);
    Diffuse[0].UseSlot(0);
    if(!Specular.empty())
        Specular[0].UseSlot(1);
    if(!Normal.empty())
        Normal[0].UseSlot(2);
    program->SetUniform(name+".ambient", Ambient);
    program->SetUniform(name+".diffuse", 0);
    program->SetUniform(name+".specular", !Specular.empty() ? 1 : 0);
    program->SetUniform(name+".HasSpecular", !Specular.empty());
    program->SetUniform(name+".normal", !Normal.empty() ? 2 : 0);
    program->SetUniform(name+".HasNormal", !Normal.empty());
    program->SetUniform(name+".shininess", Shininess);
}

void Material::UI(const std::string &name)
{
    ImGui::Separator();
    ImGui::Text(name.c_str());
    // ImGui::DragFloat3(("Diffuse##"+name).c_str(), glm::value_ptr(Diffuse));
    ImGui::DragFloat3(("Ambient##"+name).c_str(), glm::value_ptr(Ambient));
    // ImGui::DragFloat3(("Specular##"+name).c_str(), glm::value_ptr(Specular));
    ImGui::DragFloat(("Shininess##"+name).c_str(), &Shininess);
}

static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

Skybox::Skybox(const std::vector<std::string> &faces) : Program("Skybox.vs", "Skybox.fs"), texture(GL_TEXTURE_CUBE_MAP, faces)
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::Draw(const glm::mat4& projection, const Camera& cam)
{
    glDepthFunc(GL_LEQUAL);
    Program.Bind();
    glBindVertexArray(VAO);
    texture.UseSlot(0);
    Program.SetUniform("skybox", 0);
    Program.SetUniform("projection", projection);
    Program.SetUniform("view", glm::mat4(glm::mat3(cam.view)));
    glDrawArrays(GL_TRIANGLES, 0 ,36);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    Program.Unbind();
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
