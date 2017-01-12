#ifndef XENGINE_ASSET_SHADER_H
#define XENGINE_ASSET_SHADER_H

#include "core/Data.h"
#include "graphics/Graphics.h"
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

  void Initialize();

  void Finalize();

  void Apply();

  void UpdateResourceData(const eastl::string &name, DataPtr data);

  void UpdateResourceTexture(const eastl::string &name, ResourceID texture_id);

  void UpdateResourceSampler(const eastl::string &name, ResourceID sampler_id);

  void UpdateResourceBlock(const eastl::string &name, ResourceID uniform_buffer_id);

  PipelineConfig &pipeline_config() { return pipeline_config_; }

 private:
  ResourceID window_id_{kInvalidResourceID};
  ResourceID shader_id_{kInvalidResourceID};
  ResourceID pipeline_id_{kInvalidResourceID};
  ShaderConfig shader_config_;
  PipelineConfig pipeline_config_;
};

} // namespace xEngine

#endif // XENGINE_ASSET_SHADER_H
