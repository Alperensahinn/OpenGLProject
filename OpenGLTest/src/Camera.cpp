#include "Camera.h"

Camera::Camera(GLFWwindow* window)
{
    this->window = window;
    this->firstMouse = true;
}

void Camera::UpdateCamera()
{
}

void Camera::ProcessMouseInput(float xpos, float ypos)
{
    if (firstMouse == true)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

   
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
}

void Camera::Update(float deltaTime)
{
    //camera stuff
    float cameraSpeed = static_cast<float>(4 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->position += cameraSpeed * this->front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->position -= cameraSpeed * this->front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        this->position += cameraSpeed * this->up;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        this->position -= cameraSpeed * this->up;
    //camera stuff
    
}

