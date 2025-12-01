#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <iostream>
class Input
{
public:
    static float deltaTime;
    static float lastFrame;

    static float lastX;
    static float lastY;
    static bool firstMouse;

    static void Init(GLFWwindow* window);

    // 回调绑定函数
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // 处理键盘输入
    static void ProcessKeyboard(GLFWwindow* window, Camera& camera);

    // 每帧更新时间
    static void UpdateDeltaTime();
};

#endif
