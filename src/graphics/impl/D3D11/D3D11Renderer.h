#ifndef XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H
#define XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H

#if X_D3D11

#include "graphics/RendererInterface.h"

#include <D3D11.h>

namespace xEngine {

class D3D11Renderer: public RendererInterface {
 public:
  virtual void Initialize(const GraphicsConfig &config) override;

  virtual void Finalize() override;

  virtual void Render() override;

  virtual void ApplyTarget(ResourceID id) override;

  virtual void ApplyClearState(const ClearState &state) override;

  virtual void Reset() override;

 private:
  int32 swap_interval_{1};
  ID3D11Device *device_{nullptr};
  ID3D11DeviceContext *context_{nullptr};
  IDXGISwapChain *swap_chain_{nullptr};
  ID3D11Texture2D *render_target_{nullptr};
  ID3D11RenderTargetView *render_target_view_{nullptr};
  ID3D11Texture2D *depth_stencil_buffer_{nullptr};
  ID3D11DepthStencilView *depth_stencil_view_{nullptr};
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H