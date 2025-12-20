#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/Shader.h"
#include "../include/Camera.h"
#include "../include/Input.h"
#include "../include/Mesh.h"
#include "../include/Texture.h"
#include "../include/Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 设置 OpenGL 版本：3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLMiniRender - Window", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 加载 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST); // 开启深度测试，确保像素的位置覆盖关系
    // 这里的shader可以下先用绝对路径
    Shader shader("C:/C++Code/OpenGLMiniRender/shader/vertex_shader.vs", "C:/C++Code/OpenGLMiniRender/shader/fragment_shader.fs");
    // 加载模型
    const std::string modelPath = "C:/C++Code/OpenGLMiniRender/models/fazhang.obj";
    Model ourModel(modelPath);
    // 加载纹理
    Texture fazhangDiffuse("C:/C++Code/OpenGLMiniRender/textures/fazhang.png", "texture_diffuse");
    // 注入纹理
    for (unsigned int i = 0; i < ourModel.meshes.size(); i++) 
    {
        ourModel.meshes[i].textures.push_back(fazhangDiffuse);
    }
    // =======================
    // 初始化 MVP 矩阵
    // =======================
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );
    shader.use();
    shader.setMat4("projection", projectionMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("model", modelMatrix);

    // 创建 Camera（位置在 0,0,3），并且设置mvp矩阵
    Camera camera(
        glm::vec3(0.0f, 0.0f, 3.0f),     // position
        glm::vec3(0.0f, 1.0f, 0.0f),     // up 向量
        -90.0f,                          // yaw
        0.0f                             // pitch
    );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f);



    glfwSetWindowUserPointer(window, &camera);
    // 初始化输入系统
    Input::Init(window);


    while (!glfwWindowShouldClose(window))
    {
        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Input::UpdateDeltaTime();
        Input::ProcessKeyboard(window, camera);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            800.0f / 600.0f,
            0.1f,
            100.0f);

        glm::mat4 view = camera.GetViewMatrix();


        shader.use();
        shader.setMat4("projection", projection); // <--- 新增或移动
        shader.setMat4("view", view);             // <--- 新增或移动
        shader.setMat4("model", model);
        /*cubeMesh.Draw(shader);*/
        ourModel.Draw(shader);

        // 交换前后缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
