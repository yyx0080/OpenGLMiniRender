#version 330 core
// 输入
in vec2 TexCoord; // 接收从 VS 插值过来的纹理坐标。
out vec4 FragColor;
uniform sampler2D texture_diffuse1; // 假设你的漫反射纹理的 uniform 命名是 texture_diffuse1

void main()
{
    // 采样模型纹理
    vec4 texColor = texture(texture_diffuse1, TexCoord);
    // 如果纹理透明度太低则丢弃（防止透明部分显示为黑色）
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
