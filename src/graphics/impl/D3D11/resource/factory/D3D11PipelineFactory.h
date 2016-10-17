#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_FACTORY_D3D11PIPELINEFACTORY_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_FACTORY_D3D11PIPELINEFACTORY_H

#if X_D3D11

#include "graphics/impl/D3D11/resource/D3D11GraphicsResource.h"

#include "resource/ResourceFactory.h"

namespace xEngine {

class D3D11PipelineFactory: public ResourceFactory<D3D11Pipeline> {
 public:
  virtual void Create(D3D11Pipeline &resource) override;

  virtual void Destroy(D3D11Pipeline &resource) override;
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_FACTORY_D3D11PIPELINEFACTORY_H