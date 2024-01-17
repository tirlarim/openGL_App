#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //?
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glClearColor(002.0f/255.0f, 233.0f/255.0f, 161.0f/255.0f, 1.0f);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // rendering commands here
    glClear(GL_COLOR_BUFFER_BIT);
    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
  return 0;
}
