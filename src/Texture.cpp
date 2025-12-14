#include "../include/Texture.h"
#include <iostream>
#include "../external/stb_image.h" 

Texture::Texture(const std::string& path, const std::string& type)
{
    this->type = type;
    int width, height, nrComponents;
    // 1. 加载图片数据 (CPU 侧)
    unsigned char* data = LoadImage(path, width, height, nrComponents);
    if (data)
    {
        // 2. 配置和上传 OpenGL 纹理 (GPU 侧)
        setupTexture(data, width, height, nrComponents);
    }
    else
    {
        std::cerr << "ERROR::TEXTURE::Failed to load texture at path: " << path << std::endl;
        this->ID = 0; // 失败时ID设为0
    }
    // stbi_load 函数分配了内存，我们需要使用 stbi_image_free 释放
    if (data) {
        stbi_image_free(data);
    }
}


// 使用 stb_image.h 库加载图片
unsigned char* Texture::LoadImage(const std::string& path, int& width, int& height, int& nrComponents)
{
    // 翻转 Y 轴，因为 OpenGL 纹理坐标 (0,0) 通常在左下角，而图片库加载时 (0,0) 在左上角。
    stbi_set_flip_vertically_on_load(true);
    // 使用 stbi_load 加载图片。nrComponents 返回图片的颜色通道数 (3=RGB, 4=RGBA)。
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    return data;
}

// 配置 OpenGL 纹理 (setupTexture)
void Texture::setupTexture(unsigned char* data, int width, int height, int nrComponents)
{
    // 1. 自动推断纹理格式
    GLenum format;
    if (nrComponents == 1)
    {
        format = GL_RED;
    }
    else if (nrComponents == 3)
    {
        format = GL_RGB;
    }
    else if (nrComponents == 4)
    {
        format = GL_RGBA;
    }
    else 
    {
        std::cerr << "ERROR::TEXTURE::Unsupported number of components: " << nrComponents << std::endl;
        return;
    }

    // 2. 生成和绑定纹理对象
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // 3. 设置纹理环绕 (Wrapping) 和过滤 (Filtering) 模式
    // 环绕模式：通常设置为 GL_REPEAT (平铺) 或 GL_CLAMP_TO_EDGE (边缘拉伸)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 过滤模式：GL_LINEAR (线性插值，平滑) 用于放大和缩小
    // Mipmap 过滤：使用 GL_LINEAR_MIPMAP_LINEAR (最好的质量)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. 将图片数据上传到 GPU
    // GL_TEXTURE_2D | 0 (mipmap level) | format (内部格式) | width | height | 0 (边框) | format (源格式) | GL_UNSIGNED_BYTE (源数据类型) | data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // 5. 生成 Mipmap (可选但推荐)
    // Mipmap 是图像的渐进式缩小版本，用于提高远处物体的渲染性能和质量。
    glGenerateMipmap(GL_TEXTURE_2D);

    // 6. 解绑 (虽然不是必须，但保持好习惯)
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(unsigned int slot) const
{
    // 激活纹理单元
    glActiveTexture(GL_TEXTURE0 + slot);
    // 绑定到 GL_TEXTURE_2D 目标
    glBindTexture(GL_TEXTURE_2D, ID);
}

// 解绑纹理
void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
