#ifndef XENGINE_ASSET_MATERIAL_H
#define XENGINE_ASSET_MATERIAL_H

#include "Shader.h"

#include "core/Core.h"

namespace xEngine {

class Material {
 public:
  CREATE_FUNC_DECLARE(Material)

  void Apply();

  ShaderPtr shader() const { return shader_; }

  void set_shader(ShaderPtr value) { shader_ = value; }

 private:
  ShaderPtr shader_;
};

PTR_DECLARE(Material)

} // namespace xEngine

#endif // XENGINE_ASSET_MATERIAL_H