#ifndef XENGINE_ASSET_GRAPHICS_CAMERA_H
#define XENGINE_ASSET_GRAPHICS_CAMERA_H

#include "core/Core.h"
#include "core/Types.h"

#include <glm.hpp>

namespace xEngine {

class Camera {
 public:
  CREATE_FUNC_DECLARE(Camera)

  void Zoom(float32 value);

  void Rotate(float32 yaw, float32 pitch);

  void Move(float32 x, float32 y);

  void UpdateMatrix();

  const glm::mat4 &matrix() const { return matrix_; }

  void set_up_direction(const glm::vec3 &value) { up_direction_ = value; UpdateMatrix(); }

  const glm::vec3 &up_direction() const { return up_direction_; }

  void set_position(const glm::vec3 &value) { position_ = value; UpdateMatrix(); }

  const glm::vec3 &positon() const { return position_; }

  void set_target(const glm::vec3 &value) { target_ = value; UpdateMatrix(); }

  const glm::vec3 &target() const { return target_; }

 private:
  glm::vec3 up_direction_;
  glm::vec3 position_;
  glm::vec3 target_;
  glm::mat4 matrix_;
};

PTR_DECLARE(Camera)

} // namespace xEngine

#endif // XENGINE_ASSET_GRAPHICS_CAMERA_H
