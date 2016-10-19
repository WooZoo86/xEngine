#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H

#if X_D3D11

#include "graphics/config/ShaderConfig.h"
#include "graphics/config/TextureConfig.h"
#include "graphics/config/MeshConfig.h"
#include "graphics/config/PipelineConfig.h"

#include "resource/Resource.h"

#include <D3D11.h>
#include <D3Dcompiler.h>

#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/tuple.h>

namespace xEngine {

struct D3D11Shader : public Resource<ShaderConfig> {

  ID3D10Blob *vertex_blob{nullptr};

  ID3D10Blob *fragment_blob{nullptr};

  ID3D11VertexShader *vertex_shader{nullptr};

  ID3D11PixelShader *fragment_shader{nullptr};

  ID3D11ShaderReflection *vertex_reflection{nullptr};

  ID3D11ShaderReflection *fragment_reflection{nullptr};

  eastl::hash_map<eastl::string, eastl::tuple<ID3D11Buffer *, size_t>> vertex_uniform_blocks;

  eastl::hash_map<eastl::string, eastl::tuple<ID3D11Buffer *, size_t>> fragment_uniform_blocks;

  eastl::hash_map<eastl::string, uint32> vertex_texture_index;

  eastl::hash_map<eastl::string, uint32> vertex_sampler_index;

  eastl::hash_map<eastl::string, uint32> fragment_texture_index;

  eastl::hash_map<eastl::string, uint32> fragment_sampler_index;

  virtual void Reset() override {
    vertex_blob = nullptr;
    fragment_blob = nullptr;
    vertex_shader = nullptr;
    fragment_shader = nullptr;
    vertex_reflection = nullptr;
    fragment_reflection = nullptr;
    vertex_uniform_blocks.clear();
    fragment_uniform_blocks.clear();
    vertex_texture_index.clear();
    vertex_sampler_index.clear();
    fragment_texture_index.clear();
    fragment_sampler_index.clear();
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

struct D3D11Pipeline : public Resource<PipelineConfig> {

  ID3D11InputLayout *input_layout{nullptr};

  ID3D11BlendState *blend_state{nullptr};

  ID3D11DepthStencilState *depth_stencil_state{nullptr};

  ID3D11RasterizerState *rasterizer_state{nullptr};

  virtual void Reset() override {
    input_layout = nullptr;
    blend_state = nullptr;
    depth_stencil_state = nullptr;
    rasterizer_state = nullptr;
  }

};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
