#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "camera.hpp"

void processInput(GLFWwindow* window, Settings_s &settings, Shader &shader, Camera &camera);
