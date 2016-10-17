#ifndef XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H
#define XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H

#if X_D3D11

#include "graphics/RendererInterface.h"
#include "graphics/impl/D3D11/D3D11Define.h"
#include "graphics/impl/D3D11/resource/D3D11GraphicsResourceManager.h"

#include "window/Window.h"

namespace xEngine {

class WindowInterface;

class D3D11GraphicsResourceManager;

class D3D11Renderer: public RendererInterface {
 public:
   virtual void Initialize(const GraphicsConfig &config) override;

   virtual void Finalize() override;

   virtual void Render() override;

   virtual void ApplyTarget(ResourceID id = kInvalidResourceID, const ClearState &state = ClearState()) override;

   virtual void ApplyViewPort(int32 x, int32 y, int32 width, int32 height) override;

   virtual void ApplyScissor(int32 x, int32 y, int32 width, int32 height) override;

   virtual void ApplyPipeline(ResourceID id) override;

   virtual void ResetPipeline() override;

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
   WindowInterface *window() {
     static WindowInterface *pointer = nullptr;
     if (pointer == nullptr) {
       pointer = Window::GetInstance().Get(config_.window).get();
     }
     return pointer;
   }

   D3D11GraphicsResourceManager *resource_manager() {
     static D3D11GraphicsResourceManager *pointer = nullptr;
     if (pointer == nullptr) {
       pointer = static_cast<D3D11GraphicsResourceManager *>(window()->graphics()->resource_manager().get());
     }
     return pointer;
   }

 private:
  GraphicsConfig config_;

  ID3D11Device *device_{nullptr};
  ID3D11DeviceContext *context_{nullptr};
  IDXGISwapChain *swap_chain_{nullptr};

  ID3D11Texture2D *render_target_{nullptr};
  ID3D11RenderTargetView *render_target_view_{nullptr};

  ID3D11Texture2D *depth_stencil_buffer_{nullptr};
  ID3D11DepthStencilView *depth_stencil_view_{nullptr};

  D3DRendererCache cache_;

  friend class Graphics;
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H