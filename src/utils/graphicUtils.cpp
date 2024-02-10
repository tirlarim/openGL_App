#include "utils.h"
#include <glm/gtc/matrix_transform.hpp>

void setupCamera(Shader &shader, Camera* camera) {
  glm::mat4 view = glm::lookAt(camera->pos, camera->pos + camera->front, camera->up);
  shader.setMat4("view", view);
}
