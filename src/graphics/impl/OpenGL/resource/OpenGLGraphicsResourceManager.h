#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCEMANAGER_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCEMANAGER_H

#if X_OPENGL

#include "OpenGLGraphicsResource.h"
#include "graphics/GraphicsResourceManagerInterface.h"

#include "factory/OpenGLShaderFactory.h"
#include "factory/OpenGLVertexDataFactory.h"
#include "factory/OpenGLIndexDataFactory.h"
#include "factory/OpenGLTextureFactory.h"

#include "resource/ResourcePool.h"

namespace xEngine {

typedef ResourcePool<OpenGLShader, ShaderConfig> OpenGLShaderPool;
typedef ResourcePool<OpenGLVertexData, VertexDataConfig> OpenGLVertexDataPool;
typedef ResourcePool<OpenGLIndexData, IndexDataConfig> OpenGLIndexDataPool;
typedef ResourcePool<OpenGLTexture, TextureConfig> OpenGLTexturePool;

class OpenGLGraphicsResourceManager: public GraphicsResourceManagerInterface {
 public:
  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual ResourceID Create(const ShaderConfig &config) override;

  virtual ResourceID Create(const VertexDataConfig &config) override;

  virtual ResourceID Create(const IndexDataConfig &config) override;

  virtual ResourceID Create(const TextureConfig &config) override;

  virtual void Destroy(ResourceID id) override;

 private:
  OpenGLShaderPool shader_pool_;
  OpenGLVertexDataPool vertex_data_pool_;
  OpenGLIndexDataPool index_data_pool_;
  OpenGLTexturePool texture_pool_;

  OpenGLShaderFactory shader_factory_;
  OpenGLVertexDataFactory vertex_data_factory_;
  OpenGLIndexDataFactory index_data_factory_;
  OpenGLTextureFactory texture_factory_;

  friend class OpenGLRenderer;
};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCEMANAGER_H