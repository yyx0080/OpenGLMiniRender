#include "../include/Input.h"

// 静态变量初始化
float Input::deltaTime = 0.0f;
float Input::lastFrame = 0.0f;

float Input::lastX = 400.0f;
float Input::lastY = 300.0f;
bool Input::firstMouse = true;

void Input::Init(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Input::MouseCallback);
    glfwSetScrollCallback(window, Input::ScrollCallback);
}



void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 反向

    lastX = xpos;
    lastY = ypos;

    // 获取 camera 实例
    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    cam->ProcessMouseMovement(xoffset, yoffset);
}


void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    cam->ProcessMouseScroll(yoffset);
    std::cout << "Mouse moved: xoffset=" << xoffset << ", yoffset=" << yoffset << std::endl;
}


void Input::ProcessKeyboard(GLFWwindow* window, Camera& camera)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

}


void Input::UpdateDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
