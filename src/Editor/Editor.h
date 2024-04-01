#pragma once
#include "../Scenes.h"
#include "../pch.h"

void Editor(Scene* scene);
void Hierachy(Scene *scene, GLuint VAO, Texture tex, Shader& sh, const glm::mat4& proj);