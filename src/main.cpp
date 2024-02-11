#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "colors.h"
#include "types.h"
#include "utils/utils.h"
#include "consts.h"
#include "shader.hpp"
#include "utils/inputWorker.hpp"
#include "camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
  camera.ProcessMouseScroll(static_cast<f32>(yOffset));
}

void mouse_callback(GLFWwindow* window, f64 xPos, f64 yPos) {
  static f32 yaw = -90.0f, pitch = 0.0f;
  static v2 last = {
      .x = WINDOW_WIDTH / 2.0f,
      .y = WINDOW_HEIGHT / 2.0f,
  };
  static bool firstMouse = true;
  const f32 sensitivity = 0.1f;
  if (firstMouse) {
    last.x = xPos;
    last.y = yPos;
    firstMouse = false;
  }
  v2 offset = {
      .x = (f32)xPos - last.x,
      .y = last.y - (f32)yPos,
  };
  glm::vec3 direction;
  last.x = xPos;
  last.y = yPos;
  offset.x *= sensitivity;
  offset.y *= sensitivity;
  yaw += offset.x;
  pitch += offset.y;
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  camera.ProcessMouseMovement(offset.x, offset.y);
}

u32 setupVertices() {
  typedef struct Shape {
    f32* vertexPtr;
    u32* indicesPtr;
    u32 vertexSize;
    u32 indicesSize;
  } SHAPE;
  f32 squareVertices[] = {squareWithTexture};
  u32 indices[] = {0, 1, 2};
  SHAPE square = {squareVertices, indices, sizeof(squareVertices), sizeof(indices)};
  u32 vbo, vao;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, square.vertexSize, square.vertexPtr, GL_STATIC_DRAW);
  // position attribute (attribute_index, element_count, normalization, stride, offset)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);
  // texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
  glEnableVertexAttribArray(1);
  return vao;
}

// texture generating func
u32 makeTexture(TEXTURE &texture) {
  u32 textureID;
  i32 width, height, nrChannels;
  u8* data = stbi_load(texture.path.c_str(), &width, &height, &nrChannels, 0);
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, texture.isRGBA ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return textureID;
}

void setupTexture(Shader &shader) {
  const u32 textureCount = 2;
  TEXTURE textures[textureCount];
  textures[0].path = "./textures/container.jpg";
  textures[1].path = "./textures/literally-me.png";
  textures[1].isRGBA = true;
  stbi_set_flip_vertically_on_load(true);
  for (auto &texture: textures)
    texture.id = makeTexture(texture);
  for (u32 i = 0; i < textureCount; ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);
}

void makeClipMatrix(Shader &shader) {
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection;
  model = glm::rotate(model, (f32)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(45.0f), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
}

void rotateObj(Shader &shader, u32 objectIndex) {
  const glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f),
  };
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));;
  glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[objectIndex]);
  model = glm::rotate(model, glm::radians((f32)(20.0f * objectIndex + glfwGetTime() * 50.0f)),
                      glm::vec3(1.0f, 0.3f, 0.5f));
  shader.setMat4("model", model);
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
  SETTINGS settings;
  GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  initGLAD();
  checkHardware();
  Shader shader("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  u32 VAO = setupVertices();
  glClearColor(COLOR_GREEN_MAIN);
  glEnable(GL_DEPTH_TEST);
  shader.use();
  shader.setFloat("transparency", settings.transparency);
  setupTexture(shader);
  glBindVertexArray(VAO);
  makeClipMatrix(shader);
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window, settings, shader, camera);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // render container
    if (camera.isZoomChanged) {
      glm::mat4 projection =
          glm::perspective(glm::radians(camera.Zoom), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
      shader.setMat4("projection", projection);
      camera.isZoomChanged = false;
    }
    if (camera.isViewChanged) {
      glm::mat4 view = camera.GetViewMatrix();
      shader.setMat4("view", view);
      camera.isViewChanged = false;
    }
    for (u32 i = 0; i < 10; i++) {
      rotateObj(shader, i);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &VAO);
//  glDeleteBuffers(1, &VBO);
  glfwTerminate();
}

int main() {
  initGL();
  initGraphic();
  return 0;
}
