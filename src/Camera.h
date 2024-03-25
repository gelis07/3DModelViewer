#pragma once
#include "pch.h"

class Camera {
    public:
        void Input(const float DeltaTime);
        const glm::vec3& GetFront() const {return Front;}
        const glm::vec3& GetPos() const {return Pos;}
        glm::vec3& GetPos() {return Pos;}
        glm::mat4 view = glm::mat4(1.0f);
    private:
        glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 Pos = glm::vec3(0.0f);
        float lastX, lastY;
        float yaw, pitch = -90.0f;
};