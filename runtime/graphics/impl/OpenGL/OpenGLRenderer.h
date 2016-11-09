#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H

#if X_OPENGL

#include "graphics/RendererInterface.h"
#include "graphics/impl/OpenGL/OpenGLDefine.h"
#include "graphics/impl/OpenGL/resource/OpenGLGraphicsResourceManager.h"

#include "window/Window.h"

namespace xEngine {

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

  virtual void MakeCurrent() override;

  virtual void ApplyTarget(ResourceID id = kInvalidResourceID, const ClearState &state = ClearState::ClearAll()) override;

  virtual void ApplyViewPort(int32 x, int32 y, int32 width, int32 height) override;

  virtual void ApplyScissor(int32 x, int32 y, int32 width, int32 height) override;

  virtual void ApplyPipeline(ResourceID id) override;

  virtual void ResetPipeline() override;

  virtual void ApplyShader(ResourceID id) override;

  virtual void UpdateShaderUniformData(ResourceID shader_id, const eastl::string &name, DataPtr data) override;

  virtual void UpdateShaderUniformTexture(ResourceID shader_id, const eastl::string &name, ResourceID texture_id) override;

  virtual void UpdateShaderUniformBlock(ResourceID shader_id, const eastl::string &name, ResourceID uniform_buffer_id) override;

  virtual void ResetShader() override;

  virtual void UpdateUniformBufferData(ResourceID id, DataPtr data) override;

  virtual void ApplySampler(ResourceID shader_id, const eastl::string &name, ResourceID sampler_id) override;

  virtual void ResetSampler() override;

  virtual void ApplyMesh(ResourceID id) override;

  virtual void UpdateMesh(ResourceID id, DataPtr vertex_data, DataPtr index_data) override;

  virtual void ResetMesh() override;

  virtual void Draw(const DrawCallState &state) override;

  virtual void Reset() override;

 private:
  void ApplyTexture(ResourceID id, int32 index);

  void ResetTexture();

  void ApplyBlendState(const BlendState &blend_state);

  void ResetBlendState();

  void ApplyDepthStencilState(const DepthStencilState &depth_stencil_state);

  void ResetDepthStencilState();

  void ApplyRasterizerState(const RasterizerState &rasterizer_state);

  void ResetRasterizerState();

  WindowInterface *window() {
    static WindowInterface *pointer = nullptr;
    if (pointer == nullptr) {
      pointer = Window::GetInstance().Get(config_.window).get();
    }
    return pointer;
  }

  OpenGLGraphicsResourceManager *resource_manager() {
    static OpenGLGraphicsResourceManager *pointer = nullptr;
    if (pointer == nullptr) {
      pointer = static_cast<OpenGLGraphicsResourceManager *>(window()->graphics()->resource_manager().get());
    }
    return pointer;
  }

 private:
  void *context_{nullptr};
  GraphicsConfig config_;
  OpenGLRendererCache cache_;
};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLRENDERER_H