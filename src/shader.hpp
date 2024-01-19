#ifndef OPENGL_APP_SHADER_HPP
#define OPENGL_APP_SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "types.h"


class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  unsigned int ID;
  void use() const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
private:
  static void checkVertexShaderCompileErrors(u32 vsID);
  static void checkFragmentShaderCompileErrors(u32 fsID);
  void checkProgramCompileErrors() const;
};

#endif //OPENGL_APP_SHADER_HPP
