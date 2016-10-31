#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H

#if X_D3D11

#include "graphics/config/ShaderConfig.h"
#include "graphics/config/TextureConfig.h"
#include "graphics/config/MeshConfig.h"
#include "graphics/config/PipelineConfig.h"
#include "graphics/config/SamplerConfig.h"
#include "graphics/config/UniformBufferConfig.h"

#include "resource/Resource.h"

#include <D3D11.h>
#include <D3Dcompiler.h>

#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/tuple.h>

namespace xEngine {

struct D3D11Shader : public Resource<ShaderConfig> {

  struct UniformElementInfo {
    D3D_SHADER_VARIABLE_CLASS clazz;
    D3D_SHADER_VARIABLE_TYPE type;
    size_t offset{0};
    size_t size{0};
  };

  struct UniformBlockInfo {
    size_t location{0};
    size_t size{0};
    eastl::hash_map<eastl::string, UniformElementInfo> elements;
  };

  ID3D10Blob *vertex_blob{nullptr};

  ID3D10Blob *fragment_blob{nullptr};

  ID3D11VertexShader *vertex_shader{nullptr};

  ID3D11PixelShader *fragment_shader{nullptr};

  ID3D11Buffer *vertex_global_uniform_block{nullptr};

  ID3D11Buffer *fragment_global_uniform_block{nullptr};

  eastl::hash_map<eastl::string, UniformBlockInfo> vertex_uniform_block_info;

  eastl::hash_map<eastl::string, UniformBlockInfo> fragment_uniform_block_info;

  eastl::hash_map<eastl::string, uint32> vertex_texture_index;

  eastl::hash_map<eastl::string, uint32> vertex_sampler_index;

  eastl::hash_map<eastl::string, uint32> fragment_texture_index;

  eastl::hash_map<eastl::string, uint32> fragment_sampler_index;

  virtual void Reset() override {
    vertex_blob = nullptr;
    fragment_blob = nullptr;
    vertex_shader = nullptr;
    fragment_shader = nullptr;
    vertex_global_uniform_block = nullptr;
    fragment_global_uniform_block = nullptr;
    vertex_uniform_block_info.clear();
    fragment_uniform_block_info.clear();
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

  virtual void Reset() override {
    texture = nullptr;
    render_target_view = nullptr;
    depth_stencil_view = nullptr;
    shader_resource_view = nullptr;
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

struct D3D11Sampler : public Resource<SamplerConfig> {

  ID3D11SamplerState *sampler_state{nullptr};

  virtual void Reset() override {
    sampler_state = nullptr;
  }

};

struct D3D11UniformBuffer : public Resource<UniformBufferConfig> {
  
  ID3D11Buffer *uniform_buffer{nullptr};

  virtual void Reset() override {
    uniform_buffer = nullptr;
  }

};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
