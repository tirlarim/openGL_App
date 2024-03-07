#include "Camera.hpp"

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, f32 yaw, f32 pitch) :
    Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(up),
    Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  isViewChanged = isZoomChanged = true;
  updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, f32 yaw, f32 pitch) :
    Position(glm::vec3(posX, posY, posZ)), Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    WorldUp(glm::vec3(upX, upY, upZ)),
    Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  isViewChanged = isZoomChanged = true;
  updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(Position, Position + Front, Up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera-defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, f32 deltaTime) {
  isViewChanged = true;
  f32 velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD)
    Position += Front * velocity;
  if (direction == BACKWARD)
    Position -= Front * velocity;
  if (direction == LEFT)
    Position -= Right * velocity;
  if (direction == RIGHT)
    Position += Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(f32 xOffset, f32 yOffset, GLboolean constrainPitch) {
  isViewChanged = true;
  xOffset *= MouseSensitivity;
  yOffset *= MouseSensitivity;
  Yaw += xOffset;
  Pitch += yOffset;
  // make sure that when the pitch is out of bounds, the screen doesn't get flipped
  if (constrainPitch) {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }
  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

void Camera::ProcessMouseScroll(f32 yOffset) {
  isZoomChanged = true;
  Zoom -= (f32)yOffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
  // calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // also re-calculate the Right and Up vector
  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
