#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;  // 顶点法线 (本次未使用，但保留)
layout (location = 2) in vec2 aTexCoord; // 新增：纹理坐标 (Layout 2)


out vec2 TexCoord; // <-- 输出
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // 将顶点属性中的纹理坐标传递给输出变量
    TexCoord = aTexCoord;
}
