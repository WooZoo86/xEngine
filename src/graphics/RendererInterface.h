#ifndef XENGINE_GRAPHICS_GRAPHICSINTERFACE_H
#define XENGINE_GRAPHICS_GRAPHICSINTERFACE_H

#include "graphics/config/GraphicsConfig.h"
#include "graphics/state/BlendState.h"
#include "graphics/state/ClearState.h"
#include "graphics/state/DepthStencilState.h"
#include "graphics/state/RasterizerState.h"

#include "core/Data.h"
#include "resource/Resource.h"

#include <EASTL/string.h>

namespace xEngine {

class RendererInterface {
 public:
  virtual ~RendererInterface() {}

  virtual void Initialize(const GraphicsConfig &config) = 0;

  virtual void Finalize() = 0;

  virtual void MakeCurrent() = 0;

  virtual void Render() = 0;

  virtual void ApplyTarget(ResourceID id = kInvalidResourceID, const ClearState &state = ClearState()) = 0;

  virtual void ApplyViewPort(int32 x, int32 y, int32 width, int32 height) = 0;

  virtual void ApplyScissor(int32 x, int32 y, int32 width, int32 height) = 0;

  virtual void ApplyPipeline(ResourceID id) = 0;

  virtual void ResetPipeline() = 0;

  virtual void ApplyShader(ResourceID id) = 0;

  virtual void UpdateShaderUniformData(ResourceID shader_id, const eastl::string &name, DataPtr data) = 0;

  virtual void UpdateShaderUniformTexture(ResourceID shader_id, const eastl::string &name, ResourceID texture_id) = 0;

  virtual void UpdateShaderUniformBlock(ResourceID shader_id, const eastl::string &name, ResourceID uniform_buffer_id) = 0;

  virtual void ResetShader() = 0;

  virtual void UpdateUniformBufferData(ResourceID id, size_t offset, DataPtr data) = 0;

  virtual void ApplySampler(ResourceID shader_id, const eastl::string &name, ResourceID sampler_id) = 0;

  virtual void ResetSampler() = 0;

  virtual void ApplyMesh(ResourceID id) = 0;

  virtual void UpdateMesh(ResourceID id, DataPtr vertex_data, size_t vertex_offset, DataPtr index_data, size_t index_offset) = 0;

  virtual void ResetMesh() = 0;

  virtual void DrawTopology(VertexTopology topology, int32 first, int32 count) = 0;

  virtual void Reset() = 0;
};

}

#endif // XENGINE_GRAPHICS_GRAPHICSINTERFACE_H