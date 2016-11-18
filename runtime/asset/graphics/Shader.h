#ifndef XENGINE_ASSET_GRAPHICS_SHADER_H
#define XENGINE_ASSET_GRAPHICS_SHADER_H

#include "core/Data.h"
#include "graphics/GraphicsDefine.h"
#include "resource/Resource.h"

#include <EASTL/string.h>

namespace xEngine {

class Shader;

PTR_DECLARE(Shader)

class Shader {
 public:
  static ShaderPtr Parse(ResourceID window, DataPtr data);

 public:
  CREATE_FUNC_DECLARE(Shader)

  Shader();

  ~Shader();

  ResourceID GetResourceID();

  void Apply();

  void UpdateResourceData(const eastl::string &name, DataPtr data);

  void UpdateResourceTexture(const eastl::string &name, ResourceID texture_id);

  void UpdateResourceSampler(const eastl::string &name, ResourceID sampler_id);

  void UpdateResourceBlock(const eastl::string &name, ResourceID uniform_buffer_id);

 private:
  ResourceID window_id_{kInvalidResourceID};
  ResourceID resource_id_[static_cast<uint8>(GraphicsType::kCount)];
  eastl::string vertex_[static_cast<uint8>(GraphicsType::kCount)];
  eastl::string fragment_[static_cast<uint8>(GraphicsType::kCount)];
};

} // namespace xEngine

#endif // XENGINE_ASSET_GRAPHICS_SHADER_H
