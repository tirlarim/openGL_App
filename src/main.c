#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "colors.h"
#include "types.h"
#include "utils/utils.h"
#include "consts.h"

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height);
void processInput(GLFWwindow* window);

const u8 trianglesCount = 2;

void checkProgramCompilation(u32 shaderProgram) {
  int success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, MAX_ERR_LOG_LEN, NULL, infoLog);
    fprintf(stderr, "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s\n", infoLog);
    abort();
  }
}

void checkShaderCompilation(u32 shader) {
  int success;
  char infoLog[MAX_ERR_LOG_LEN];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, MAX_ERR_LOG_LEN, NULL, infoLog);
    fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    abort();
  }
}

u32 compileShaderProgram() { // rewrite as cycle
  typedef struct ShaderFile {
    u32 type;
    char path[MAX_FILE_PATH_LEN];
  } SHADER_FILE;
  const u32 shadersCount = 2;
  u32 shaderProgram = glCreateProgram();
  SHADER_FILE shaderFiles[shadersCount] = { // add shaders here
      {GL_VERTEX_SHADER,   "./shaders/vertexShader.vert"},
      {GL_FRAGMENT_SHADER, "./shaders/fragmentShader.frag"},
  };
  for (u8 i = 0; i < shadersCount; ++i) {
    FILE* shaderFile = fopen(shaderFiles[i].path, "rt");
    i64 shaderFileSize = getFileSize(shaderFile) + 1;
    u32 shader;
    const char* shaderSource = calloc(shaderFileSize, sizeof(char));
    fread((char*)shaderSource, shaderFileSize, 1, shaderFile);
    shader = glCreateShader(shaderFiles[i].type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    checkShaderCompilation(shader);
    glAttachShader(shaderProgram, shader);
    glDeleteShader(shader);
    free((char*)shaderSource);
  }
  glLinkProgram(shaderProgram);
  checkProgramCompilation(shaderProgram);
  return shaderProgram;
}

u32* setupVertices() {
  typedef struct Shape {
    f32* vertexPtr;
    u32 size;
  }SHAPE;
  f32 triangle1[] = {
      -0.8f, -0.4f, 0.0f,
      -0.4f, 0.4f, 0.0f,
      -0.0f, -0.4f, 0.0f,
  };
  f32 triangle2[] = {
      0.0f, -0.4f, 0.0f,
      0.4f, 0.4f, 0.0f,
      0.8f, -0.4f, 0.0f,
  };
  SHAPE triangles[trianglesCount] = {
      {triangle1, sizeof(triangle1)},
      {triangle2, sizeof(triangle2)},
  };
  static u32 vao[trianglesCount];
  u32 vbo[trianglesCount];
  glGenBuffers(trianglesCount, vbo);
  glGenVertexArrays(trianglesCount, vao);
  for (u8 i = 0; i < trianglesCount; ++i) {
    glBindVertexArray(vao[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    glBufferData(GL_ARRAY_BUFFER, triangles[i].size, triangles[i].vertexPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
  }
  return vao;
}

void initGraphic() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow* window = glfwCreateWindow(SCR.WIDTH, SCR.HEIGHT, "Test OpenGL App", NULL, NULL);
  u32 shaderProgram;
  u32* VAOs;
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    abort();
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    abort();
  }
  glClearColor(COLOR_GREEN_MAIN);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  shaderProgram = compileShaderProgram();
  VAOs = setupVertices();
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    for (u8 i = 0; i < trianglesCount; ++i) {
      glBindVertexArray(VAOs[i]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  const static u32 allDrawModes[] = {GL_POINT, GL_LINE, GL_FILL};
  static u8 currentModeIndex = 0;
  static bool spaceKeyPressed = false;
  bool spaceDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
  if (spaceKeyPressed && !spaceDown) glPolygonMode(GL_FRONT_AND_BACK, allDrawModes[(++currentModeIndex) % 3]);
  spaceKeyPressed = spaceDown;
}

int main() {
  initGraphic();
  return 0;
}
