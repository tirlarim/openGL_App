#include "inputWorker.h"
#include <glm/gtc/matrix_transform.hpp>
#include "types.h"

void processInput(GLFWwindow* window, SETTINGS* settings, Shader &shader) {
  struct keys_s {
    bool space, esc, up, down, w, a, s, d, q, e;
  } keysDown = {0};
  static struct keys_s keysPressed = {0};
  const u32 drawModes[] = {GL_FILL, GL_LINE}; // GL_POINT looks like void screen, because pixels too small
  const f32 transparencyStep = 0.01f;
  const f32 transparencyMin = 0.0f;
  const f32 transparencyMax = 1.0f;
  const u32 drawModesLen = (sizeof(drawModes) / sizeof(*drawModes));
  static u8 currentModeIndex = 0;
  static v3 viewOffset = {0.0f, 0.0f, 0.0f};
  keysDown.space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  keysDown.esc = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
  keysDown.up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
  keysDown.down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
  keysDown.w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
  keysDown.a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
  keysDown.s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
  keysDown.d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
  keysDown.e = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
  keysDown.q = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
  if (keysDown.esc) glfwSetWindowShouldClose(window, true);
  if (keysPressed.space && !keysDown.space)
    glPolygonMode(GL_FRONT_AND_BACK, drawModes[++currentModeIndex % drawModesLen]);
  if (keysDown.up && settings->transparency < transparencyMax) {
    settings->transparency += transparencyStep;
    shader.setFloat("transparency", settings->transparency);
  }
  if (keysDown.down && settings->transparency > transparencyMin) {
    settings->transparency -= transparencyStep;
    shader.setFloat("transparency", settings->transparency);
  }
  if (keysDown.w) viewOffset.z += 0.05f;
  if (keysDown.s) viewOffset.z -= 0.05f;
  if (keysDown.a) viewOffset.x += 0.05f;
  if (keysDown.d) viewOffset.x -= 0.05f;
  if (keysDown.q) viewOffset.y += 0.05f;
  if (keysDown.e) viewOffset.y -= 0.05f;
  if (keysDown.w || keysDown.a || keysDown.s || keysDown.d || keysDown.e || keysDown.q) {
    glm::mat4 view = glm::translate(
        glm::mat4(1.0f), glm::vec3(0.0f + viewOffset.x, 0.0f + viewOffset.y, -3.0f + viewOffset.z));
    shader.setMat4("view", view);
  }
  keysPressed = keysDown;
}