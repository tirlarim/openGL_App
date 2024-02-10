#ifndef OPENGL_APP_INPUTWORKER_H
#define OPENGL_APP_INPUTWORKER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../shader.hpp"

void processInput(GLFWwindow* window, SETTINGS* settings, Shader &shader);

#endif //OPENGL_APP_INPUTWORKER_H
