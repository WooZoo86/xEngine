#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H

#if X_D3D11

#include "D3D11GraphicsResource.h"
#include "graphics/GraphicsResourceManagerInterface.h"

#include "factory/D3D11ShaderFactory.h"
#include "factory/D3D11TextureFactory.h"
#include "factory/D3D11MeshFactory.h"
#include "factory/D3D11PipelineFactory.h"

#include "resource/ResourcePool.h"

namespace xEngine {

typedef ResourcePool<D3D11Shader, ShaderConfig> D3D11ShaderPool;
typedef ResourcePool<D3D11Texture, TextureConfig> D3D11TexturePool;
typedef ResourcePool<D3D11Mesh, MeshConfig> D3D11MeshPool;
typedef ResourcePool<D3D11Pipeline, PipelineConfig> D3D11PipelinePool;

class D3D11GraphicsResourceManager: public GraphicsResourceManagerInterface {
 public:
  explicit D3D11GraphicsResourceManager(ID3D11Device *device) :
    shader_factory_(device),
    texture_factory_(device),
    mesh_factory_(device),
    pipeline_factory_(this, device){}

  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual ResourceID Create(const ShaderConfig &config) override;

  virtual ResourceID Create(const TextureConfig &config) override;

  virtual ResourceID Create(const MeshConfig &config) override;

  virtual ResourceID Create(const PipelineConfig &config) override;

  virtual void Destroy(ResourceID id) override;

 private:
  D3D11ShaderPool shader_pool_;
  D3D11TexturePool texture_pool_;
  D3D11MeshPool mesh_pool_;
  D3D11PipelinePool pipeline_pool_;

  D3D11ShaderFactory shader_factory_;
  D3D11TextureFactory texture_factory_;
  D3D11MeshFactory mesh_factory_;
  D3D11PipelineFactory pipeline_factory_;

  friend class D3D11Renderer;
  friend class D3D11PipelineFactory;
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H
