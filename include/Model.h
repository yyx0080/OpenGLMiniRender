#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <map> // 用于缓存加载过的纹理

#include "Mesh.h"   
#include "Shader.h" 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// 缓存已加载的纹理，避免重复加载同一图片
unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
    // 成员变量
    std::vector<Texture> textures_loaded; // 存储已加载的纹理（避免重复加载）
    std::vector<Mesh> meshes;            // 存储模型包含的所有网格对象
    std::string directory;               // 模型文件所在的目录路径 (用于定位纹理文件)
    bool gammaCorrection;                // Gamma校正标志

    // 构造函数：接受文件路径，并自动启动加载
    Model(const std::string& path, bool gamma = false);

    // 渲染函数：循环调用所有 Mesh 的 Draw 方法
    void Draw(const Shader& shader) const;

private:
    // 核心加载函数
    void loadModel(const std::string& path);

    // 递归处理 Assimp 节点树
    void processNode(aiNode* node, const aiScene* scene);

    // 将 Assimp 的 aiMesh 结构体转换为我们自己的 Mesh 类
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // 从 Assimp 材质中加载纹理
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif