#ifndef OPENGL_APP_INPUTWORKER_HPP
#define OPENGL_APP_INPUTWORKER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../shader.hpp"
#include "../camera.h"

void processInput(GLFWwindow* window, SETTINGS &settings, Shader &shader, Camera &camera);

#endif //OPENGL_APP_INPUTWORKER_HPP
