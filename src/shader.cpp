#include <glm/gtc/type_ptr.hpp>
#include "consts.h"
#include "shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  // __Retrieve the vertex/fragment source code from filePath__
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    std::stringstream vShaderStream, fShaderStream;
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  // __Compile shaders__
  unsigned int vertex, fragment;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  checkShaderCompileErrors(vertex, GL_VERTEX_SHADER);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  checkShaderCompileErrors(fragment, GL_FRAGMENT_SHADER);
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkProgramCompileErrors();
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() const {
  glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, i32 value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, f32 value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec3(const std::string &name, f32 value1, f32 value2, f32 value3) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkShaderCompileErrors(u32 shaderID, u32 shaderType) {
  i32 success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, MAX_ERR_LOG_LEN, nullptr, infoLog);
    switch (shaderType) {
      case GL_VERTEX_SHADER:
        std::cerr << "ERROR::VERTEX_SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
        break;
      case GL_GEOMETRY_SHADER:
        std::cerr << "ERROR::GEOMETRY_SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
        break;
      case GL_FRAGMENT_SHADER:
        std::cerr << "ERROR::FRAGMENT_SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
        break;
      default:
        std::cerr << "ERROR::UNKNOWN_SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
        break;
    }
  }
}

void Shader::checkProgramCompileErrors() const {
  i32 success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, MAX_ERR_LOG_LEN, nullptr, infoLog);
    std::cerr << "ERROR::PROGRAM_COMPILATION_ERROR\n" << infoLog << std::endl;
  }
}
