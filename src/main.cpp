#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "colors.h"
#include "types.h"
#include "utils/utils.h"
#include "consts.h"
#include "shader.hpp"
#include "stb_image.h"

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height);
void processInput(GLFWwindow* window);
void checkHardware();

u32 setupVertices() {
  typedef struct Shape {
    f32* vertexPtr;
    u32* indicesPtr;
    u32 size;
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
  SHAPE square = {squareVertices, indices, sizeof(squareVertices)};
  u32 vbo, vao, ebo;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, square.size, square.vertexPtr, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, square.size, square.indicesPtr, GL_STATIC_DRAW);
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
u32 makeTexture(const char* texturePath, bool isRGBA = false) {
  u32 texture;
  i32 width, height, nrChannels;
  u8* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (data) {
    if (isRGBA)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return texture;
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

GLFWwindow* createWindow(i32 width, i32 height, const std::string &windowTitle) {
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
  typedef struct Texture {
    u32 id = {0};
    std::string path;
    bool isRGBA = false;
  } TEXTURE;
  const u32 textureCount = 2;
  TEXTURE textures[textureCount];
  textures[0].path = "./textures/wall.jpg";
  textures[1].path = "./textures/literally-me.png";
  textures[1].isRGBA = true;
  GLFWwindow* window = createWindow(800, 600, WINDOW_NAME);
  initGLAD();
  checkHardware();
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  Shader shader("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");
  u32 VAO = setupVertices();
  stbi_set_flip_vertically_on_load(true);
  for (auto & texture : textures)
    texture.id = makeTexture(texture.path.c_str(), texture.isRGBA);
  glClearColor(COLOR_GREEN_MAIN);
  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
  shader.setInt("texture2", 1); // or with shader class
  for (u32 i = 0; i < textureCount; ++i) {
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT);
    // render container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
  const u32 allDrawModes[] = {GL_POINT, GL_LINE, GL_FILL};
  static u8 currentModeIndex = 0;
  static bool spaceKeyPressed = false;
  bool spaceDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
  if (spaceKeyPressed && !spaceDown) {
    currentModeIndex = (++currentModeIndex) % 3;
    glPolygonMode(GL_FRONT_AND_BACK, allDrawModes[currentModeIndex]);
  }
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
