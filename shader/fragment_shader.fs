#version 330 core
// 输入
in vec2 TexCoord; // 接收从 VS 插值过来的纹理坐标
// -------------------- Uniform --------------------
// 声明一个纹理采样器。
// 这个 sampler2D 对应 Mesh::Draw 中绑定的 GL_TEXTURE0、GL_TEXTURE1 等纹理单元。
uniform sampler2D texture_diffuse1; // 假设你的漫反射纹理的 uniform 命名是 texture_diffuse1

out vec4 FragColor;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord);
}
