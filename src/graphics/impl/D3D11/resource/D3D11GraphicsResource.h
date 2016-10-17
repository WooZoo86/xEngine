#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H

#if X_D3D11

#include "graphics/config/ShaderConfig.h"
#include "graphics/config/TextureConfig.h"
#include "graphics/config/MeshConfig.h"

#include "resource/Resource.h"

#include <D3D11.h>

namespace xEngine {

struct D3D11Shader : public Resource<ShaderConfig> {

  ID3D11VertexShader *vertex_shader{nullptr};

  ID3D11PixelShader *fragment_shader{nullptr};

  virtual void Reset() override {
    vertex_shader = nullptr;
    fragment_shader = nullptr;
  }

};

struct D3D11Texture : public Resource<TextureConfig> {

  ID3D11Texture2D *texture{nullptr};

  ID3D11RenderTargetView *render_target_view{nullptr};

  ID3D11DepthStencilView *depth_stencil_view{nullptr};

  ID3D11ShaderResourceView *shader_resource_view{nullptr};

  ID3D11SamplerState *sampler_state{nullptr};

  virtual void Reset() override {
    texture = nullptr;
    render_target_view = nullptr;
    depth_stencil_view = nullptr;
    shader_resource_view = nullptr;
    sampler_state = nullptr;
  }

};

struct D3D11Mesh : public Resource<MeshConfig> {

  ID3D11Buffer *vertex_buffer{nullptr};

  ID3D11Buffer *index_buffer{nullptr};

  virtual void Reset() override {
    vertex_buffer = nullptr;
    index_buffer = nullptr;
  }

};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
