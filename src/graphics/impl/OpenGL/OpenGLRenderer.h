#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H

#if X_OPENGL

#include "graphics/RendererInterface.h"

#include <glad/glad.h>

namespace xEngine {

class OpenGLRenderer : public RendererInterface {
 public:
  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual void ApplyTarget(ResourceID id) override;

  virtual void ApplyClearState(const ClearState &state) override;

  virtual void Reset() override;

#ifdef GLAD_DEBUG
  static void PreOpenGLCallback(const char *name, void *funcptr, int len_args, ...);

  static void PostOpenGLCallback(const char *name, void *funcptr, int len_args, ...);
#endif

};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H