#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "types.h"


class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  u32 ID;
  void use() const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, i32 value) const;
  void setFloat(const std::string &name, f32 value) const;
  void setVec3(const std::string &name, glm::vec3 value) const;
  void setVec3(const std::string &name, f32 value1, f32 value2, f32 value3) const;
  void setMat4(const std::string &name, const glm::mat4 &value) const;
// setMaterial "name" should be a uniform name
  void setMaterial(const std::string &name, const Material material) const;
private:
  static void checkShaderCompileErrors(u32 shaderID, u32 shaderType);
  void checkProgramCompileErrors() const;
};
