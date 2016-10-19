#if X_D3D11

#include "D3D11ShaderFactory.h"

#include "graphics/impl/D3D11/D3D11Define.h"

#include "core/Log.h"

namespace xEngine {

static ID3D10Blob *CompileShader(const char *type, const char *source) {
  ID3D10Blob *shader = nullptr;
  ID3D10Blob *log = nullptr;
  auto flag = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_WARNINGS_ARE_ERRORS;
#if X_DEBUG
  flag |= D3D10_SHADER_DEBUG;
#else
  vertex_flag |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

  Log::GetInstance().Info("[compile shader]\n%s\n", source);

  if (FAILED(D3DCompile(
    source,
    strlen(source),
    nullptr,
    nullptr,
    nullptr,
    "main",
    type,
    flag,
    0,
    &shader,
    &log
  ))) {
    if (log != nullptr) {
      Log::GetInstance().Error("compile error: %s\n", log->GetBufferPointer());
      log->Release();
      log = nullptr;
    }
    if (shader != nullptr) {
      shader->Release();
      shader = nullptr;
    }
  }

  return shader;
}

void D3D11ShaderFactory::Create(D3D11Shader &resource) {
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();

  if (resource.config().vertex == nullptr || resource.config().fragment == nullptr) {
    Log::GetInstance().Error("vertex or fragment is nullptr\n");
    resource.Failed();
    return;
  }

  auto vertex_blob = CompileShader("vs_5_0", resource.config().vertex);
  auto fragment_blob = CompileShader("ps_5_0", resource.config().fragment);

  ID3D11VertexShader *vertex_shader = nullptr;
  ID3D11PixelShader *fragment_shader = nullptr;

  if (vertex_blob != nullptr) {
    x_d3d11_assert_msg(device_->CreateVertexShader(
      vertex_blob->GetBufferPointer(),
      vertex_blob->GetBufferSize(),
      nullptr,
      &vertex_shader
    ), "create vertex shader failed\n");

    ID3D11ShaderReflection *vertex_reflection = nullptr;
    x_d3d11_assert_msg(D3DReflect(
      vertex_blob->GetBufferPointer(),
      vertex_blob->GetBufferSize(),
      IID_ID3D11ShaderReflection,
      reinterpret_cast<void **>(&vertex_reflection)
    ), "create vertex reflect failed\n");

    resource.vertex_reflection = vertex_reflection;

    D3D11_SHADER_DESC vertex_desc;
    x_d3d11_assert_msg(vertex_reflection->GetDesc(&vertex_desc), "get vertex description failed\n");

    auto vertex_uniform_count = 0;
    auto vertex_texture_count = 0;

    for (auto resource_index = 0; resource_index < vertex_desc.BoundResources; ++resource_index) {
      D3D11_SHADER_INPUT_BIND_DESC resource_desc;
      x_d3d11_assert_msg(vertex_reflection->GetResourceBindingDesc(resource_index, &resource_desc), "get resource description failed\n");

      if (resource_desc.Type == D3D_SIT_CBUFFER) {
        ++vertex_uniform_count;

        x_assert(vertex_uniform_count < static_cast<uint16>(GraphicsMaxDefine::kMaxUniformBlockCount));

        auto uniform_block_reflection = vertex_reflection->GetConstantBufferByName(resource_desc.Name);

        D3D11_SHADER_BUFFER_DESC uniform_block_desc;
        x_d3d11_assert_msg(uniform_block_reflection->GetDesc(&uniform_block_desc), "get constant reflection failed\n");

        ID3D11Buffer *uniform_buffer = nullptr;

        D3D11_BUFFER_DESC uniform_buffer_desc;
        ZeroMemory(&uniform_buffer_desc, sizeof(uniform_buffer_desc));

        uniform_buffer_desc.ByteWidth = uniform_block_desc.Size;
        uniform_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        uniform_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        uniform_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        x_d3d11_assert_msg(device_->CreateBuffer(&uniform_buffer_desc, nullptr, &uniform_buffer), "create uniform buffer failed\n");

        resource.vertex_uniform_blocks.insert({ uniform_block_desc.Name, eastl::make_tuple(uniform_buffer, uniform_block_desc.Size) });
      }
      else if (resource_desc.Type == D3D_SIT_TEXTURE) {
        ++vertex_texture_count;
        x_assert(vertex_uniform_count < static_cast<uint16>(GraphicsMaxDefine::kMaxTextureCount));
        resource.vertex_texture_index.insert(eastl::make_pair(resource_desc.Name, resource_desc.BindPoint));
      }
      else if (resource_desc.Type == D3D_SIT_SAMPLER) {
        resource.vertex_sampler_index.insert(eastl::make_pair(resource_desc.Name, resource_desc.BindPoint));
      }
    }
  }

  if (fragment_blob != nullptr) {
    x_d3d11_assert_msg(device_->CreatePixelShader(
      fragment_blob->GetBufferPointer(),
      fragment_blob->GetBufferSize(),
      nullptr,
      &fragment_shader
    ), "create fragment shader failed\n");

    ID3D11ShaderReflection *fragment_reflection = nullptr;
    x_d3d11_assert_msg(D3DReflect(
      fragment_blob->GetBufferPointer(),
      fragment_blob->GetBufferSize(),
      IID_ID3D11ShaderReflection,
      reinterpret_cast<void **>(&fragment_reflection)
    ), "create fragment reflect failed\n");

    resource.fragment_reflection = fragment_reflection;

    D3D11_SHADER_DESC fragment_desc;
    x_d3d11_assert_msg(fragment_reflection->GetDesc(&fragment_desc), "get fragment description failed\n");

    auto fragment_uniform_count = 0;
    auto fragment_texture_count = 0;

    for (auto resource_index = 0; resource_index < fragment_desc.BoundResources; ++resource_index) {
      D3D11_SHADER_INPUT_BIND_DESC resource_desc;
      x_d3d11_assert_msg(fragment_reflection->GetResourceBindingDesc(resource_index, &resource_desc), "get resource description failed\n");

      if (resource_desc.Type == D3D_SIT_CBUFFER) {
        ++fragment_uniform_count;

        x_assert(fragment_uniform_count < static_cast<uint16>(GraphicsMaxDefine::kMaxUniformBlockCount));

        auto uniform_block_reflection = fragment_reflection->GetConstantBufferByName(resource_desc.Name);

        D3D11_SHADER_BUFFER_DESC uniform_block_desc;
        x_d3d11_assert_msg(uniform_block_reflection->GetDesc(&uniform_block_desc), "get constant reflection failed\n");

        ID3D11Buffer *uniform_buffer = nullptr;

        D3D11_BUFFER_DESC uniform_buffer_desc;
        ZeroMemory(&uniform_buffer_desc, sizeof(uniform_buffer_desc));

        uniform_buffer_desc.ByteWidth = uniform_block_desc.Size;
        uniform_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        uniform_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        uniform_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        x_d3d11_assert_msg(device_->CreateBuffer(&uniform_buffer_desc, nullptr, &uniform_buffer), "create uniform buffer failed\n");

        resource.fragment_uniform_blocks.insert({ uniform_block_desc.Name, eastl::make_tuple(uniform_buffer, uniform_block_desc.Size) });
      }
      else if (resource_desc.Type == D3D_SIT_TEXTURE) {
        ++fragment_texture_count;
        x_assert(fragment_uniform_count < static_cast<uint16>(GraphicsMaxDefine::kMaxTextureCount));
        resource.fragment_texture_index.insert(eastl::make_pair(resource_desc.Name, resource_desc.BindPoint));
      }
      else if (resource_desc.Type == D3D_SIT_SAMPLER) {
        resource.fragment_sampler_index.insert(eastl::make_pair(resource_desc.Name, resource_desc.BindPoint));
      }
    }
  }

  resource.vertex_blob = vertex_blob;
  resource.fragment_blob = fragment_blob;
  resource.vertex_shader = vertex_shader;
  resource.fragment_shader = fragment_shader;

  resource.Complete();
}

void D3D11ShaderFactory::Destroy(D3D11Shader &resource) {
  x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
  if (resource.vertex_blob != nullptr) {
    resource.vertex_blob->Release();
  }
  if (resource.fragment_blob != nullptr) {
    resource.fragment_blob->Release();
  }
  if (resource.vertex_shader != nullptr) {
    resource.vertex_shader->Release();
  }
  if (resource.fragment_shader != nullptr) {
    resource.fragment_shader->Release();
  }
  if (resource.vertex_reflection != nullptr) {
    resource.vertex_reflection->Release();
  }
  if (resource.fragment_reflection != nullptr) {
    resource.fragment_reflection->Release();
  }
  for (auto &pair : resource.vertex_uniform_blocks) {
    auto uniform_buffer = eastl::get<0>(pair.second);
    if (uniform_buffer != nullptr) {
      uniform_buffer->Release();
    }
  }
  for (auto &pair : resource.fragment_uniform_blocks) {
    auto uniform_buffer = eastl::get<0>(pair.second);
    if (uniform_buffer != nullptr) {
      uniform_buffer->Release();
    }
  }
}

} // namespace xEngine

#endif // X_D3D11
