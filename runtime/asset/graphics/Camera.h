#ifndef XENGINE_ASSET_GRAPHICS_CAMERA_H
#define XENGINE_ASSET_GRAPHICS_CAMERA_H

#include "core/Core.h"
#include "core/Types.h"
#include "resource/Resource.h"

#include <glm.hpp>

#pragma push_macro("near")
#pragma push_macro("far")

#undef near
#undef far

namespace xEngine {

enum class CameraType {
  kPerspective,
  kOrthographic,
};

class Camera {
 public:
  CREATE_FUNC_DECLARE(Camera)

  void Rotate(float32 yaw, float32 pitch);

  void Move(float32 x, float32 y);

  void set_zoom(float32 value) { zoom_ = glm::max(0.0f, value); UpdateProjectionMatrix(); }

  float32 zoom() const { return zoom_; }

  void set_render_window(ResourceID value) { render_window_ = value; UpdateProjectionMatrix(); }

  ResourceID render_window() const { return render_window_; }

  void set_type(CameraType value) { type_ = value; UpdateProjectionMatrix(); }

  CameraType type() const { return type_; }

  void set_fov(float32 value) { fov_ = value; UpdateProjectionMatrix(); }

  float32 fov() const { return fov_; }

  void set_size(float32 value) { size_ = value; UpdateProjectionMatrix(); }

  float32 size() const { return size_; }

  void set_near(float32 value) { near_ = value; UpdateProjectionMatrix(); }

  float32 near() const { return near_; }

  void set_far(float32 value) { far_ = value; UpdateProjectionMatrix(); }

  float32 far() const { return far_; }

  const glm::mat4 &projection_matrix() const { return projection_matrix_; }

  void set_up_direction(const glm::vec3 &value) { up_direction_ = value; UpdateViewMatrix(); }

  const glm::vec3 &up_direction() const { return up_direction_; }

  void set_position(const glm::vec3 &value) { position_ = value; UpdateViewMatrix(); }

  const glm::vec3 &positon() const { return position_; }

  void set_target(const glm::vec3 &value) { target_ = value; UpdateViewMatrix(); }

  const glm::vec3 &target() const { return target_; }

  const glm::mat4 &view_matrix() const { return view_matrix_; }

 private:
  void UpdateProjectionMatrix();

  void UpdateViewMatrix();

 private:
  float32 zoom_{1.0f};

  ResourceID render_window_{kInvalidResourceID};

  CameraType type_{CameraType::kPerspective};

  float32 fov_{60.0f}; // perspective
  float32 size_{5.0f}; // orthographic

  float32 near_{1.0f};
  float32 far_{1000.0f};

  glm::mat4 projection_matrix_;

  glm::vec3 up_direction_;
  glm::vec3 position_;
  glm::vec3 target_;

  glm::mat4 view_matrix_;
};

PTR_DECLARE(Camera)

} // namespace xEngine

#pragma pop_macro("near")
#pragma pop_macro("far")

#endif // XENGINE_ASSET_GRAPHICS_CAMERA_H
