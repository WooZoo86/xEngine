#ifndef XENGINE_GRAPHICS_GRAPHICSINTERFACE_H
#define XENGINE_GRAPHICS_GRAPHICSINTERFACE_H

#include "graphics/Graphics.h"
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

  virtual void Render() = 0;

  virtual void ApplyTarget(ResourceID id = kInvalidResourceID) = 0;

  virtual void ApplyClearState(const ClearState &state) = 0;

  virtual void ApplyBlendState(const BlendState &blend_state) = 0;

  virtual void ResetBlendState() = 0;

  virtual void ApplyDepthStencilState(const DepthStencilState &depth_stencil_state) = 0;

  virtual void ResetDepthStencilState() = 0;

  virtual void ApplyRasterizerState(const RasterizerState &rasterizer_state) = 0;

  virtual void ResetRasterizerState() = 0;

  virtual void ApplyViewPort(int32 x, int32 y, int32 width, int32 height) = 0;

  virtual void ApplyScissor(int32 x, int32 y, int32 width, int32 height) = 0;

  virtual void ApplyShader(ResourceID id) = 0;

  virtual void UpdateShaderUniform(ResourceID id, eastl::string name, UniformFormat format, const void *buffer) = 0;

  virtual void ResetShader() = 0;

  virtual void ApplyVertexData(ResourceID id, bool force_update = false) = 0;

  virtual void UpdateVertexData(ResourceID id, int32 offset, DataPtr data) = 0;

  virtual void ResetVertexData() = 0;

  virtual void ApplyIndexData(ResourceID id) = 0;

  virtual void UpdateIndexData(ResourceID id, int32 offset, DataPtr data) = 0;

  virtual void ResetIndexData() = 0;

  virtual void ApplyTexture(ResourceID id, int32 index) = 0;

  virtual void ResetTexture() = 0;

  virtual void DrawTopology(VertexTopology topology, int32 first, int32 count) = 0;

  virtual void Reset() = 0;
};

}

#endif // XENGINE_GRAPHICS_GRAPHICSINTERFACE_H