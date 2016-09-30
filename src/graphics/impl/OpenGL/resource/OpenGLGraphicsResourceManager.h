#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCEMANAGER_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCEMANAGER_H

#if X_OPENGL

#include "OpenGLGraphicsResource.h"
#include "graphics/GraphicsResourceManagerInterface.h"

#include "factory/OpenGLShaderFactory.h"

#include <resource/ResourcePool.h>

namespace xEngine {

typedef ResourcePool<OpenGLShader, ShaderConfig> OpenGLShaderPool;

class OpenGLGraphicsResourceManager : public GraphicsResourceManagerInterface {
 public:
  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual ResourceID Create(const ShaderConfig &config) override;

  virtual void Destroy(ResourceID id) override;

 private:
  OpenGLShaderPool shader_pool_;
  OpenGLShaderFactory shader_factory_;
};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCEMANAGER_H