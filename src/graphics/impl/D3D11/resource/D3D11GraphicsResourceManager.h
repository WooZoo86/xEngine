#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H

#if X_D3D11

#include "graphics/GraphicsResourceManagerInterface.h"

namespace xEngine {

class D3D11GraphicsResourceManager: public GraphicsResourceManagerInterface {

 public:
  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual ResourceID Create(const ShaderConfig &config) override;

  virtual void Destroy(ResourceID id) override;
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H