#include "../include/Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::setupMesh()
{
    // 生成 VAO, VBO 和 EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 只在有索引数据时，才生成 EBO，这样做可以生成两个模式的Mesh，使用EBO的与不使用EBO
    if (!this->indices.empty()) 
    {
        glGenBuffers(1, &EBO);
    }
    else 
    {
        EBO = 0; // 明确EBO未初始化
    }
    // 绑定 VAO
    glBindVertexArray(VAO);
    // 绑定 VBO 并上传顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 这里需要解释一下，实际上顶点数据的大小应该是vertices数量的多少 * Vertex这个数据结构的大小
    // 并且需要取得vertices的首地址
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
    // 绑定 EBO 并上传索引数据
    // 条件化绑定 EBO 并上传索引数据
    if (!this->indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // 如果 indices 非空，则安全地获取首地址并上传数据
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
    }
    // 步长是单个 Vertex 结构体的总大小
    GLsizei stride = sizeof(Vertex);
    // 设置顶点位置
    glEnableVertexAttribArray(0);
    // 3个float, 步长为整个Vertex大小, 偏移量为 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    // 法线向量 (Layout 1)
    glEnableVertexAttribArray(1);
    // 3个float, 步长为整个Vertex大小, 偏移量是 Position 之后
    // 3 * sizeof(float) 是 Position 的大小
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    // 纹理坐标 (Layout 2)
    glEnableVertexAttribArray(2);
    // 2个float, 步长为整个Vertex大小, 偏移量是 Position + Normal 之后
    // (3 + 3) * sizeof(float) 是 Position 和 Normal 的大小
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    // 6. 解绑 VAO (必须最后解绑VAO，而不是VBO/EBO)
    glBindVertexArray(0);
}


void Mesh::Draw(const Shader& shader) const
{
    // 1. 绑定纹理
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // 激活纹理单元 i

        std::string name = textures[i].GetType();
        std::string number;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        // 设置 Shader 中的 Uniform 变量 (例如：texture_diffuse1)
        shader.setInt((name + number).c_str(), i);

        glBindTexture(GL_TEXTURE_2D, textures[i].ID); // 绑定纹理 ID
    }

    // 2. 绘制网格
    glBindVertexArray(VAO);
    if (!this->indices.empty())
    {
        // 索引绘制 (如果 Mesh 有索引数据)
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        // 数组绘制 (如果 Mesh 没有索引数据)
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size()); // 使用顶点数组的大小
    }
    glBindVertexArray(0);
    // 3. 恢复默认纹理状态 (可选，但推荐)
    glActiveTexture(GL_TEXTURE0);
}