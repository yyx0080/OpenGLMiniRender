#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 LightingColor; // 接收来自 VS 插值后的颜色

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(LightingColor, 1.0) * texColor;
}