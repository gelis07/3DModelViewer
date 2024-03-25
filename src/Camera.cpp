#include "Camera.h"

void Camera::Input(const float DeltaTime)
{
    double xpos, ypos;
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;
        // if(pitch > 89.0f)
        //     pitch = 89.0f;
        // if(pitch < -89.0f)
        //     pitch = -89.0f;
        Direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        Direction.y = glm::sin(glm::radians(pitch));
        Direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        Front = glm::normalize(Direction);
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }else {
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W)){
        Pos += Front * 10.0f * DeltaTime;
    }else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S))
        Pos -= Front * 10.0f * DeltaTime;
    lastX = xpos;
    lastY = ypos;
    view = glm::lookAt(Pos, Pos + Front, glm::vec3(0.0f, 1.0f, 0.0f));
}