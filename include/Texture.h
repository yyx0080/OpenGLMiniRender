#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID; // OpenGL 纹理对象 ID

    // 构造函数：加载图片文件并配置 OpenGL 纹理
    Texture(const std::string& path, const std::string& type);

    // 绑定纹理到指定的纹理单元（Texture Unit）
    void Bind(unsigned int slot = 0) const;

    // 解绑纹理（可选）
    void Unbind() const;

    // 获取纹理类型（用于Shader中的Uniform变量命名）
    std::string GetType() const { return type; }

private:
    std::string type; // 例如: "texture_diffuse", "texture_specular"

    // 内部函数：加载图片数据
    unsigned char* LoadImage(const std::string& path, int& width, int& height, int& nrComponents);

    // 内部函数：配置 OpenGL 纹理参数
    void setupTexture(unsigned char* data, int width, int height, int nrComponents);
};

#endif