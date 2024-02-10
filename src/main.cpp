#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "colors.h"
#include "types.h"
#include "utils/utils.h"
#include "consts.h"
#include "shader.hpp"

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height);
void processInput(GLFWwindow* window, SETTINGS* settings, Shader &shader);

u32 setupVertices() {
  typedef struct Shape {
    f32* vertexPtr;
    u32* indicesPtr;
    u32 vertexSize;
    u32 indicesSize;
  } SHAPE;
  f32 squareVertices[] = { // positions[3], colors[3], texture_coords[2]
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
  };
  u32 indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  SHAPE square = {squareVertices, indices, sizeof(squareVertices), sizeof(indices)};
  u32 vbo, vao, ebo;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, square.vertexSize, square.vertexPtr, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, square.indicesSize, square.indicesPtr, GL_STATIC_DRAW);
  // position attribute (attribute_index, element_count, normalization, stride, offset)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
  glEnableVertexAttribArray(2);
  return vao;
}

// texture generating func
u32 makeTexture(TEXTURE& texture) {
  u32 textureID;
  i32 width, height, nrChannels;
  u8* data = stbi_load(texture.path.c_str(), &width, &height, &nrChannels, 0);
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, texture.isRGBA ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return textureID;
}

void matrixTransform(Shader &shader) {
  auto trans = glm::mat4(1.0f);
  i32 transformLoc = glGetUniformLocation(shader.ID, "transform");
  f32 time = (f32)glfwGetTime();
  f32 timeSin = sin(time);
  trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));
  trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
  trans = glm::scale(trans, glm::vec3(timeSin, timeSin, timeSin));
  shader.use();
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void makeClipMatrix(Shader &shader) {
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection;
  i32 transformModel = glGetUniformLocation(shader.ID, "model");
  i32 transformView = glGetUniformLocation(shader.ID, "view");
  i32 transformProjection = glGetUniformLocation(shader.ID, "projection");
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(45.0f), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
  glUniformMatrix4fv(transformModel, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(transformView, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(transformProjection, 1, GL_FALSE, glm::value_ptr(projection));
}

void matrixScaleByTime(Shader &shader) {
  auto trans = glm::mat4(1.0f);
  i32 transformLocation = glGetUniformLocation(shader.ID, "transform");
  trans = glm::rotate(trans, -(f32)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
  trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 1.0f));
  shader.use();
  glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
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

GLFWwindow* createWindow(u32 width, u32 height, const std::string &windowTitle) {
  GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    abort();
  }
  glfwMakeContextCurrent(window);
  return window;
}

void initGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    abort();
  }
}

void initGraphic() {
  const u32 textureCount = 2;
  TEXTURE textures[textureCount];
  SETTINGS settings;
  textures[0].path = "./textures/container.jpg";
  textures[1].path = "./textures/literally-me.png";
  textures[1].isRGBA = true;
  GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  initGLAD();
  checkHardware();
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  Shader shader("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");
  u32 VAO = setupVertices();
  stbi_set_flip_vertically_on_load(true);
  for (auto &texture : textures)
    texture.id = makeTexture(texture);
  glClearColor(COLOR_GREEN_MAIN);
  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);
  shader.setFloat("transparency", settings.transparency);
  for (u32 i = 0; i < textureCount; ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glBindVertexArray(VAO);
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window, &settings, shader);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT);
    // render container
    makeClipMatrix(shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, SETTINGS* settings, Shader &shader) {
  const u32 drawModes[] = {GL_FILL, GL_LINE}; // GL_POINT looks like void screen, because pixels too small
  const f32 transparencyStep = 0.01f;
  const f32 transparencyMin = 0.0f;
  const f32 transparencyMax = 1.0f;
  const u32 drawModesLen = (sizeof(drawModes)/sizeof(*drawModes));
  static u8 currentModeIndex = 0;
  static bool spaceKeyPressed = false;
  bool spaceDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  bool escDown = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
  bool wDown = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
  bool sDown = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
  if (escDown) glfwSetWindowShouldClose(window, true);
  if (spaceKeyPressed && !spaceDown) glPolygonMode(GL_FRONT_AND_BACK, drawModes[++currentModeIndex % drawModesLen]);
  if (wDown && settings->transparency < transparencyMax) settings->transparency += transparencyStep;
  if (sDown && settings->transparency > transparencyMin) settings->transparency -= transparencyStep;
  spaceKeyPressed = spaceDown;
  shader.setFloat("transparency", settings->transparency);
}

int main() {
  initGL();
  initGraphic();
  return 0;
}
