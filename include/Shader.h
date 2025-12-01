#pragma once
#include <string>
#include <glad/glad.h>
#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;

    // uniform ¹¤¾ßº¯Êý
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};
