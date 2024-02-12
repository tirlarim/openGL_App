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
#include "camera.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
u32 VAO, VBO; // rewrite ?
u32 lightVAO;
SETTINGS settings;

void frameBufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
  if (!settings.pause)
    camera.ProcessMouseScroll(static_cast<f32>(yOffset));
}

void mouse_callback(GLFWwindow* window, f64 xPos, f64 yPos) {
  static bool firstMouse = true;
  if (!settings.pause) {
    static v2 last = {
        .x = WINDOW_WIDTH / 2.0f,
        .y = WINDOW_HEIGHT / 2.0f,
    };
    v2 pos = {
        .x = static_cast<f32>(xPos),
        .y = static_cast<f32>(yPos),
    };
    if (firstMouse) {
      last = pos;
      firstMouse = false;
    }
    camera.ProcessMouseMovement(pos.x - last.x, last.y - pos.y); // diff
    last = pos;
  } else {
    firstMouse = true;
  }
}

void setupVertices() {
  typedef struct Shape {
    f32* vertexPtr;
    u32* indicesPtr;
    u32 vertexSize;
    u32 indicesSize;
  } SHAPE;
  f32 squareVertices[] = VertexSquareWithNormalVec;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
  // position attribute (attribute_index, element_count, element_type, normalization, stride, offset)
  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);
//  normal vector data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(f32), (void*)(3*sizeof(f32)));
  glEnableVertexAttribArray(1);
// light position attribute
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);
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

void setupEffects(Shader &shader) {
  shader.use();
//  shader.setFloat("transparency", settings.transparency);
  shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
}

void makeClipMatrix(Shader &shader) {
  shader.use();
  glm::mat4 identityMat4 = glm::mat4(1.0f);
  glm::mat4 model = glm::rotate(identityMat4, (f32)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
  glm::mat4 view = glm::translate(identityMat4, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
}

void rotateObj(Shader &shader, u32 objectIndex) {
  const glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f), glm::vec3(1.5f, 2.0f, -2.5f), glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f), glm::vec3(2.4879f, 3.7567f, 0.3648f), glm::vec3(-0.5576f, 1.7177f, -1.5196f),
  };
  const auto cubePositionsLen = sizeof(cubePositions)/sizeof(*cubePositions);
  if (objectIndex >= cubePositionsLen) throw("fn -> rotateObj: objectIndex overflow");
  glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1.0f), cubePositions[objectIndex]),
                                glm::radians((20.0f * (f32)objectIndex + (f32)glfwGetTime() * 50.0f)),
                                glm::vec3(1.0f, 0.3f, 0.5f));
  shader.setMat4("model", model);
}

void setupLight(Shader &shader, const glm::vec3 &lightPos) {
  shader.use();
  glm::mat4 identityMat4 = glm::mat4(1.0f);
  glm::mat4 model = glm::scale(glm::translate(identityMat4, lightPos), glm::vec3(0.2f));
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
  GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  initGLAD();
  checkHardware();
  Shader shader("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");
  Shader lightShader("./shaders/vertexShader.vert", "./shaders/lightShader.frag");
  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  setupVertices();
  glClearColor(COLOR_GREEN_MAIN);
  glEnable(GL_DEPTH_TEST);
//  setupTexture(shader);
  glBindVertexArray(VAO);
  makeClipMatrix(shader);
  makeClipMatrix(lightShader);
  setupEffects(shader);
  setupLight(lightShader, lightPos);
  shader.use();
  shader.setVec3("lightPos", lightPos);
  while (!glfwWindowShouldClose(window)) {
    // input
    shader.use();
    processInput(window, settings, shader, camera);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setVec3("lightPos", lightPos);
    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    // render container
    if (!settings.pause) {
      if (camera.isZoomChanged) {
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.Zoom), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
        shader.use();
        shader.setMat4("projection", projection);
        lightShader.use();
        lightShader.setMat4("projection", projection);
        camera.isZoomChanged = false;
      }
      if (camera.isViewChanged) {
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("view", view);
        lightShader.use();
        lightShader.setMat4("view", view);
        camera.isViewChanged = false;
      }
    }
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // also draw the lamp object
    lightShader.use();
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
}

int main() {
  initGL();
  initGraphic();
  return 0;
}
