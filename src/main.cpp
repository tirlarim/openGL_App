#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "colors.h"
#include "types.h"
#include "utils/utils.h"
#include "consts.h"
#include "shader.hpp"

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height);
void processInput(GLFWwindow* window);
void checkHardware();

const u8 trianglesCount = 2;

u32* setupVertices() {
  typedef struct Shape {
    f32* vertexPtr;
    u32 size;
  } SHAPE;
  f32 triangle1[] = { // positions[3], colors[3]
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
  };
  f32 triangle2[] = {
      -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
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
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
  return vao;
}

void initGL() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* createWindow(i32 width, i32 height, const std::string& windowTitle) {
  GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    abort();
  }
  glfwMakeContextCurrent(window);
  return window;
}

void initGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    abort();
  }
}

void initGraphic() {
  GLFWwindow* window = createWindow(800, 600, WINDOW_NAME);
  initGLAD();
  checkHardware();
  glClearColor(COLOR_GREEN_MAIN);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  Shader shader("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");
  u32* VAOs = setupVertices();
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // rendering commands here
    shader.use();
    shader.setFloat("ourColor", static_cast<float>(glfwGetTime()) / 2.0f + 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);
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

void checkHardware() {
  printGraphicsCardInfo();
  printOpenGLLimits();
}

int main() {
  initGL();
  initGraphic();
  return 0;
}
