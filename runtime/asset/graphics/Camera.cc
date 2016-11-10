#include "Camera.h"

#include <gtc/matrix_transform.hpp>

namespace xEngine {

void Camera::Zoom(float32 value) {
  auto eye_direction = target_ - position_;
  position_ += value * glm::normalize(eye_direction);
  UpdateMatrix();
}

void Camera::Rotate(float32 yaw, float32 pitch) {
  auto eye_direction = target_ - position_;
  auto rotation = glm::rotate(glm::mat4(), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation = glm::rotate(rotation, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  target_ = glm::vec3(rotation * glm::vec4(eye_direction, 0.0f)) + position_;
  UpdateMatrix();
}

void Camera::Move(float32 x, float32 y) {
  auto eye_direction = target_ - position_;
  auto right_direction = glm::cross(up_direction_, eye_direction);
  position_ = position_ + x * glm::normalize(right_direction) + y * glm::normalize(up_direction_);
  target_ = position_ + eye_direction;
  UpdateMatrix();
}

void Camera::UpdateMatrix() {
  matrix_ = glm::lookAt(position_, target_, up_direction_);
}

} // namespace xEngine
