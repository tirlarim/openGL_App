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
  checkVertexShaderCompileErrors(vertex);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  checkFragmentShaderCompileErrors(fragment);
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

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkVertexShaderCompileErrors(u32 vsID) {
  int success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetShaderiv(vsID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vsID, MAX_ERR_LOG_LEN, nullptr, infoLog);
    std::cerr << "ERROR::VERTEX_SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
  }
}

void Shader::checkFragmentShaderCompileErrors(u32 fsID) {
  int success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetShaderiv(fsID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fsID, MAX_ERR_LOG_LEN, nullptr, infoLog);
    std::cerr << "ERROR::FRAGMENT_SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
  }
}

void Shader::checkProgramCompileErrors() const {
  int success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, MAX_ERR_LOG_LEN, nullptr, infoLog);
    std::cerr << "ERROR::PROGRAM_COMPILATION_ERROR\n" << infoLog << std::endl;
  }
}

