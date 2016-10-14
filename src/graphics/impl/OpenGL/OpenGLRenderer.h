#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H

#if X_OPENGL

#include "OpenGLDefine.h"
#include "graphics/RendererInterface.h"

#include <glad/glad.h>

namespace xEngine {

class WindowInterface;

class OpenGLGraphicsResourceManager;

class OpenGLRenderer: public RendererInterface {
 private:
#ifdef GLAD_DEBUG
  static void PreOpenGLCallback(const char *name, void *function, int len_args, ...);

  static void PostOpenGLCallback(const char *name, void *function, int len_args, ...);
#endif

 public:
  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual void Render() override;

  virtual void ApplyTarget(ResourceID id = kInvalidResourceID, const ClearState &state = ClearState()) override;

  virtual void ApplyViewPort(int32 x, int32 y, int32 width, int32 height) override;

  virtual void ApplyScissor(int32 x, int32 y, int32 width, int32 height) override;

  virtual void ApplyBlendState(const BlendState &blend_state) override;

  virtual void ResetBlendState() override;

  virtual void ApplyDepthStencilState(const DepthStencilState &depth_stencil_state) override;

  virtual void ResetDepthStencilState() override;

  virtual void ApplyRasterizerState(const RasterizerState &rasterizer_state) override;

  virtual void ResetRasterizerState() override;

  virtual void ApplyShader(ResourceID id) override;

  virtual void UpdateShaderUniform(ResourceID id, eastl::string name, UniformFormat format, const void *buffer) override;

  virtual void ResetShader() override;

  virtual void ApplyTexture(ResourceID id, int32 index) override;

  virtual void ResetTexture() override;

  virtual void ApplyMesh(ResourceID id) override;

  virtual void UpdateMesh(ResourceID id,
                          const void *vertex_buffer,
                          size_t vertex_offset,
                          size_t vertex_size,
                          const void *index_buffer,
                          size_t index_offset,
                          size_t index_size) override;

  virtual void ResetMesh() override;

  virtual void DrawTopology(VertexTopology topology, int32 first, int32 count) override;

  virtual void Reset() override;

 private:
  GraphicsConfig config_;
  OpenGLRendererCache cache_;
  OpenGLGraphicsResourceManager *resource_manager_{nullptr};
  WindowInterface *window_{nullptr};
};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H