#ifndef OPENGL_APP_CAMERA_HPP
#define OPENGL_APP_CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "types.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
const f32 YAW = -90.0f;
const f32 PITCH = 0.0f;
const f32 SPEED = 2.5f;
const f32 SENSITIVITY = 0.1f;
const f32 ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  f32 Yaw;
  f32 Pitch;
  // camera options
  f32 MovementSpeed;
  f32 MouseSensitivity;
  f32 Zoom;
  bool isViewChanged;
  bool isZoomChanged;
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), f32 yaw = YAW, f32 pitch = PITCH);
  Camera(f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, f32 yaw, f32 pitch);
  glm::mat4 GetViewMatrix();
  void ProcessKeyboard(Camera_Movement direction, f32 deltaTime);
  void ProcessMouseMovement(f32 xOffset, f32 yOffset, GLboolean constrainPitch = true);
  void ProcessMouseScroll(f32 yOffset);
private:
  void updateCameraVectors();
};

#endif //OPENGL_APP_CAMERA_HPP
