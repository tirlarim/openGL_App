#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

#include "colors.h"
#include "types.h"
#include "utils/utils.h"
#include "consts.h"
#include "shader.hpp"
#include "inputWorker.hpp"
#include "camera.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
u32 VAO, VBO; // rewrite ?
u32 lightVAO;
Settings settings;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
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
  f32 squareVertices[] = VertexSquareNormalTexture;
  const u32 stride = 3 + 3 + 2;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
  // position attribute (attribute_index, element_count, element_type, normalization, stride, offset)
  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);
//  normal vector data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(f32), (void*)(3 * sizeof(f32)));
  glEnableVertexAttribArray(1);
//  texture data
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(f32), (void*)(6 * sizeof(f32)));
  glEnableVertexAttribArray(2);
// light position attribute
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(f32), NULL);
  glEnableVertexAttribArray(0);
}

// texture generating func
u32 makeTexture(const char* texture) {
  u32 textureID;
  i32 width, height, nrChannels;
  u8* data = stbi_load(texture, &width, &height, &nrChannels, 0);
  GLenum format;
  switch (nrChannels) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      format = GL_RGB;
      break;
  }
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return textureID;
}

void setupTexture(Shader &shader) {
  const u32 textureCount = 2;
  Texture textures[textureCount];
  textures[0].path = "./textures/container2.png";
  textures[1].path = "./textures/container2_specular.png";
  std::string lightTypes[2] = {
      "material.diffuse",
      "material.specular"
  };
  stbi_set_flip_vertically_on_load(true);
  for (u32 i = 0; i < textureCount; ++i) {
    std::cout << "Load texture: " << textures[i].path.c_str() << std::endl;
    textures[i].id = makeTexture(textures[i].path.c_str());
  }
  shader.use();
  for (u32 i = 0; i < textureCount; ++i) {
    shader.setInt(lightTypes[i], i);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
}

void setupEffects(Shader &shader) {
  shader.use();
  shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
  shader.setVec3("light.ambient", glm::vec3(0.2f));
  shader.setVec3("light.diffuse", glm::vec3(0.5f)); // darken diffuse light a bit
  shader.setVec3("light.specular", glm::vec3(1.0f));
}

void changeColorOverTime(Shader &shader) {
  f32 time = glfwGetTime();
  glm::vec3 lightColor(sin(time * 2.0f), sin(time * 0.7f), sin(time * 1.3f));
  glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
  glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
  shader.use();
  shader.setVec3("light.ambient", ambientColor);
  shader.setVec3("light.diffuse", diffuseColor);
}

void makeClipMatrix(Shader &shader) {
  shader.use();
  glm::mat4 identityMat4 = glm::mat4(1.0f);
  glm::mat4 model = identityMat4;
  glm::mat4 view = glm::translate(identityMat4, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.01f, 100.0f);
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
  const auto cubePositionsLen = sizeof(cubePositions) / sizeof(*cubePositions);
  if (objectIndex >= cubePositionsLen) throw ("fn -> rotateObj: objectIndex overflow");
  glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1.0f), cubePositions[objectIndex]),
                                glm::radians((20.0f * (f32)objectIndex + (f32)glfwGetTime() * 50.0f)),
                                glm::vec3(1.0f, 0.3f, 0.5f));
  shader.setMat4("model", model);
}

void moveLight(Shader &lightShader, Shader &objShader, glm::vec3 &lightPos) {
  const f32 radius = 1.5f;
  const f32 rotateCoefficient = 180.0f / static_cast<f32>(M_PI);
  lightPos = glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius);
  glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f));
  model = glm::rotate(model,
                      glm::radians(atan(lightPos.x / lightPos.z) * rotateCoefficient),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  lightShader.use();
  lightShader.setMat4("model", model);
  objShader.use();
  objShader.setVec3("light.position", lightPos);
}

void setMaterial(Shader &shader, const std::string &materialName = "default") {
  if (materialName == "default") {
    shader.use();
    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 64.0f);
    return;
  }
  const u8 vec3Len = 3;
  static std::vector<Material> materials;
  bool status = false;
//#define LogMaterials
  if (materials.empty()) {
#ifdef LogMaterials
    std::cout << "Parse materials:" << std::endl;
#endif
    Material tmpMaterial;
    std::string line;
    std::ifstream materialFileList("./materials/data.csv");
    if (!materialFileList.is_open()) std::cerr << "Unable to open materials file -> ./materials/data.csv" << std::endl;
    while (std::getline(materialFileList, line)) {
      std::istringstream lineStream(line);
      std::getline(lineStream, tmpMaterial.name, '\t');
      for (u8 i = 0; i < vec3Len; ++i) lineStream >> tmpMaterial.ambient[i];
      for (u8 i = 0; i < vec3Len; ++i) lineStream >> tmpMaterial.diffuse[i];
      for (u8 i = 0; i < vec3Len; ++i) lineStream >> tmpMaterial.specular[i];
      lineStream >> tmpMaterial.shininess;
      materials.push_back(tmpMaterial);
#ifdef LogMaterials
      std::cout << line << std::endl;
#endif
#undef LogMaterials
    }
  }
  for (auto &item: materials) {
    if (item.name == materialName) {
      shader.setMaterial("material", item);
      status = true;
      break;
    }
  }
  if (!status) std::cerr << "Unable to set material: " << materialName << std::endl;
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
  Shader lightShader("./shaders/lightShader.vert", "./shaders/lightShader.frag");
  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  setupVertices();
  glClearColor(COLOR_DARK_BG_MAIN);
  glEnable(GL_DEPTH_TEST);
  setupTexture(shader);
  glBindVertexArray(VAO);
  makeClipMatrix(shader);
  makeClipMatrix(lightShader);
  setupEffects(shader); // light settings
  shader.use();
  shader.setVec3("light.position", lightPos);
  shader.setVec3("viewPos", camera.Position);
  setMaterial(shader);
  while (!glfwWindowShouldClose(window)) {
    // input
    shader.use();
    processInput(window, settings, shader, camera);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // render container
    moveLight(lightShader, shader, lightPos);
    if (!settings.pause) {
//      changeColorOverTime(shader);
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
        shader.setVec3("viewPos", camera.Position);
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
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
}

int main() {
  initGL();
  initGraphic();
  return 0;
}
