#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_FACTORY_OPENGLSHADERFACTORY_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_FACTORY_OPENGLSHADERFACTORY_H

#include "graphics/impl/OpenGL/resource/OpenGLGraphicsResource.h"

#include <resource/ResourceFactory.h>

namespace xEngine {

class OpenGLShaderFactory : public ResourceFactory<OpenGLShader> {
 public:
  virtual void Create(OpenGLShader &resource, DataPtr data) override;

  virtual void Destroy(OpenGLShader &resource) override;
};

}

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_FACTORY_OPENGLSHADERFACTORY_H