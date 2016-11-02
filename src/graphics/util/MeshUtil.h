#ifndef XENGINE_GRAPHICS_UTIL_MESHUTIL_H
#define XENGINE_GRAPHICS_UTIL_MESHUTIL_H

#include "graphics/config/MeshConfig.h"

#include "core/Data.h"

namespace xEngine {

class MeshUtil {
 public:
  static MeshUtil Cube(bool uv);

  static MeshUtil Sphere(bool uv, size_t divisions);

  static MeshUtil Capsule();

  static MeshUtil Cylinder();

  static MeshUtil Plane(bool uv);

 public:
  MeshUtil &BeginVertex();

  MeshUtil &Vertex(VertexElementSemantic semantic, size_t index, float32 x);

  MeshUtil &Vertex(VertexElementSemantic semantic, size_t index, float32 x, float32 y);

  MeshUtil &Vertex(VertexElementSemantic semantic, size_t index, float32 x, float32 y, float32 z);

  MeshUtil &Vertex(VertexElementSemantic semantic, size_t index, float32 x, float32 y, float32 z, float32 w);

  MeshUtil &EndVertex();

  MeshUtil &BeginIndex();

  MeshUtil &Index16(size_t index, uint16 value);

  MeshUtil &Index32(size_t index, uint32 value);

  MeshUtil &Triangle16(size_t index, uint16 a, uint16 b, uint16 c);

  MeshUtil &Triangle32(size_t index, uint32 a, uint32 b, uint32 c);

  MeshUtil &Quad16(size_t index, uint16 a, uint16 b, uint16 c, uint16 d);

  MeshUtil &Quad32(size_t index, uint32 a, uint32 b, uint32 c, uint32 d);

  MeshUtil &EndIndex();

  MeshConfig &config() { return config_; }

 private:
  template<typename T> T Convert(float32 value);

  template<typename T> T ConvertNormalized(float32 value);

 private:
  MeshConfig config_;
  DataPtr vertex_data_;
  DataPtr index_data_;
};

template<typename T>
T MeshUtil::Convert(float32 value) {
  return static_cast<T>(glm::clamp(value,
                                   static_cast<float32>(eastl::numeric_limits<T>::min()),
                                   static_cast<float32>(eastl::numeric_limits<T>::max())));
}
template<typename T>
T MeshUtil::ConvertNormalized(float32 value) {
  return static_cast<T>(glm::round(
      glm::clamp(value, -1.0f, 1.0f) * static_cast<float32>(eastl::numeric_limits<T>::max())));
}

} // namespace xEngine

#endif // XENGINE_GRAPHICS_UTIL_MESHUTIL_H