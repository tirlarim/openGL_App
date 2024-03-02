#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "Camera.hpp"

void processInput(GLFWwindow* window, Settings &settings, Shader &shader, Camera &camera);
