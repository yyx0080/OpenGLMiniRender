#ifndef MESH_H
#define MESH_H

#include <vector>
#include "../external/glm/glm.hpp"
#include "Shader.h" 
#include "../include/Texture.h"

// 顶点
struct Vertex {
    glm::vec3 Position;  // 顶点坐标
    glm::vec3 Normal;    // 法线向量（用于光照计算）
    glm::vec2 TexCoords; // 纹理坐标（用于贴图）
};

//// 纹理
//struct Texture {
//    unsigned int id;   // OpenGL 纹理 ID
//    std::string type;  // 纹理类型：e.g., "texture_diffuse", "texture_specular"
//    std::string path;  // 纹理文件路径
//};
class Texture;

class Mesh {
public:
    // 几何数据
    std::vector<Vertex>       vertices; // 顶点数据集合
    std::vector<unsigned int> indices; // 
    std::vector<Texture>      textures; // 纹理数据集合

    // 构造函数
    // 接受构成 Mesh 所需的所有数据
    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<Texture>& textures);

    // 渲染函数：用于绘制 Mesh，接受一个Shader，并且完成Mesh的绘制
    void Draw(const Shader& shader) const;

private:
    unsigned int VAO, VBO, EBO;

    // 配置 VAO/VBO/EBO 的函数
    // 构造函数调用这个函数来设置所有的 OpenGL 缓冲区
    void setupMesh();
};

#endif