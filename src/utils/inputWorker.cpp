#include "inputWorker.hpp"
#include "./utils.h"

void processInput(GLFWwindow* window, SETTINGS &settings, Shader &shader, Camera &camera) {
  struct keys_s {
    bool space, esc, up, down, w, a, s, d, q, e, shift;
  } keysDown = {0};
  static struct keys_s keysPressed = {0};
  static u8 currentModeIndex = 0;
  static f32 lastFrame = 0.0f;
  static f32 currentFrame = 0.0f;
  const u32 drawModes[] = {GL_FILL, GL_LINE}; // GL_POINT looks like void screen, because pixels too small
  const f32 transparencyStep = 0.01f, transparencyMin = 0.0f, transparencyMax = 1.0f;
  const u32 drawModesLen = (sizeof(drawModes) / sizeof(*drawModes));
  f32 deltaTime;
  currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
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
  keysDown.shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
  if (keysDown.esc) glfwSetWindowShouldClose(window, true);
  if (keysPressed.space && !keysDown.space)
    glPolygonMode(GL_FRONT_AND_BACK, drawModes[++currentModeIndex % drawModesLen]);
  if (keysDown.up && settings.transparency < transparencyMax) {
    settings.transparency += transparencyStep;
    shader.setFloat("transparency", settings.transparency);
  }
  if (keysDown.down && settings.transparency > transparencyMin) {
    settings.transparency -= transparencyStep;
    shader.setFloat("transparency", settings.transparency);
  }
  if (keysDown.shift)
    deltaTime *= 2.0f;
  if (keysDown.w)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keysDown.s)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keysDown.a)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keysDown.d)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  keysPressed = keysDown;
}
