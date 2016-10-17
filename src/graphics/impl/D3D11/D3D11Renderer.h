#ifndef XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H
#define XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H

#if X_D3D11

#include "D3D11Define.h"
#include "graphics/RendererInterface.h"

namespace xEngine {

class D3D11Renderer: public RendererInterface {
 public:
	virtual void Initialize(const GraphicsConfig &config) override;

	virtual void Finalize() override;

	virtual void Render() override;

	virtual void ApplyTarget(ResourceID id = kInvalidResourceID) override;

	virtual void ApplyClearState(const ClearState &state) override;

	virtual void ApplyBlendState(const BlendState &blend_state) override;

	virtual void ResetBlendState() override;

	virtual void ApplyDepthStencilState(const DepthStencilState &depth_stencil_state) override;

	virtual void ResetDepthStencilState() override;

	virtual void ApplyRasterizerState(const RasterizerState &rasterizer_state) override;

	virtual void ResetRasterizerState() override;

	virtual void ApplyViewPort(int32 x, int32 y, int32 width, int32 height) override;

	virtual void ApplyScissor(int32 x, int32 y, int32 width, int32 height) override;

	virtual void ApplyShader(ResourceID id) override;

	virtual void UpdateShaderUniform(ResourceID id, eastl::string name, UniformFormat format, const void *buffer) override;

	virtual void ResetShader() override;

	virtual void ApplyVertexData(ResourceID id, bool force_update = false) override;

	virtual void UpdateVertexData(ResourceID id, int32 offset, DataPtr data) override;

	virtual void ResetVertexData() override;

	virtual void ApplyIndexData(ResourceID id) override;

	virtual void UpdateIndexData(ResourceID id, int32 offset, DataPtr data) override;

	virtual void ResetIndexData() override;

	virtual void ApplyTexture(ResourceID id, int32 index) override;

	virtual void ResetTexture() override;

	virtual void DrawTopology(VertexTopology topology, int32 first, int32 count) override;

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

	friend class Graphics;
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_D3D11RENDERER_H