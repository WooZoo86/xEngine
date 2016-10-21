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

static void ReflectShader(ID3D11Device *device, ID3D10Blob *blob, ID3D11Buffer **buffer,
                          eastl::hash_map<eastl::string, D3D11Shader::UniformBlockInfo> &block_map,
                          eastl::hash_map<eastl::string, uint32> &texture_map,
                          eastl::hash_map<eastl::string, uint32> sampler_map) {
  ID3D11ShaderReflection *reflection = nullptr;

  x_d3d11_assert_msg(D3DReflect(
    blob->GetBufferPointer(),
    blob->GetBufferSize(),
    IID_ID3D11ShaderReflection,
    reinterpret_cast<void **>(&reflection)
  ), "create reflect failed\n");

  D3D11_SHADER_DESC desc;
  x_d3d11_assert_msg(reflection->GetDesc(&desc), "get description failed\n");

  auto uniform_count = 0;
  auto texture_count = 0;

  for (auto resource_index = 0; resource_index < desc.BoundResources; ++resource_index) {
    D3D11_SHADER_INPUT_BIND_DESC resource_desc;
    x_d3d11_assert_msg(reflection->GetResourceBindingDesc(resource_index, &resource_desc), "get resource description failed\n");

    if (resource_desc.Type == D3D_SIT_CBUFFER) {
      ++uniform_count;
      x_assert(uniform_count < static_cast<uint16>(GraphicsMaxDefine::kMaxUniformBlockCount));

      D3D11Shader::UniformBlockInfo info;
      info.location = resource_index;

      auto uniform_block_reflection = reflection->GetConstantBufferByName(resource_desc.Name);

      D3D11_SHADER_BUFFER_DESC uniform_block_desc;
      x_d3d11_assert_msg(uniform_block_reflection->GetDesc(&uniform_block_desc), "get constant reflection failed\n");
      info.size = uniform_block_desc.Size;

      for (auto variable_index = 0; variable_index < uniform_block_desc.Variables; ++variable_index) {
        auto variable_reflection = uniform_block_reflection->GetVariableByIndex(variable_index);
        D3D11_SHADER_VARIABLE_DESC variable_desc;
        x_d3d11_assert_msg(variable_reflection->GetDesc(&variable_desc), "get variable description failed\n");
        auto variable_type_reflection = variable_reflection->GetType();
        D3D11_SHADER_TYPE_DESC variable_type_desc;
        x_d3d11_assert_msg(variable_type_reflection->GetDesc(&variable_type_desc), "get variable type description failed\n");

        D3D11Shader::UniformElementInfo element_info;
        element_info.clazz = variable_type_desc.Class;
        element_info.type = variable_type_desc.Type;
        element_info.rows = variable_type_desc.Rows;
        element_info.column = variable_type_desc.Columns;
        element_info.offset = variable_desc.StartOffset;
        element_info.size = variable_desc.Size;

        info.elements.insert({ variable_desc.Name, element_info });
      }

      block_map.insert({ uniform_block_desc.Name, info });

      if (strcmp(uniform_block_desc.Name, "$Globals") == 0 && uniform_block_desc.Size > 0) {
        ID3D11Buffer *uniform_buffer = nullptr;

        D3D11_BUFFER_DESC uniform_buffer_desc;
        ZeroMemory(&uniform_buffer_desc, sizeof(uniform_buffer_desc));

        uniform_buffer_desc.ByteWidth = uniform_block_desc.Size;
        uniform_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        uniform_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        uniform_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        x_d3d11_assert_msg(device->CreateBuffer(&uniform_buffer_desc, nullptr, &uniform_buffer), "create global uniform buffer failed\n");

        *buffer = uniform_buffer;
      }
    }
    else if (resource_desc.Type == D3D_SIT_TEXTURE) {
      ++texture_count;
      x_assert(uniform_count < static_cast<uint16>(GraphicsMaxDefine::kMaxTextureCount));
      texture_map.insert(eastl::make_pair(resource_desc.Name, resource_desc.BindPoint));
    }
    else if (resource_desc.Type == D3D_SIT_SAMPLER) {
      sampler_map.insert(eastl::make_pair(resource_desc.Name, resource_desc.BindPoint));
    }
  }

  reflection->Release();
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

    ReflectShader(device_, vertex_blob, &resource.vertex_global_uniform_block,
      resource.vertex_uniform_block_info,
      resource.vertex_texture_index,
      resource.vertex_sampler_index);
  }

  if (fragment_blob != nullptr) {
    x_d3d11_assert_msg(device_->CreatePixelShader(
      fragment_blob->GetBufferPointer(),
      fragment_blob->GetBufferSize(),
      nullptr,
      &fragment_shader
    ), "create fragment shader failed\n");

    ReflectShader(device_, fragment_blob, &resource.fragment_global_uniform_block,
      resource.fragment_uniform_block_info,
      resource.fragment_texture_index,
      resource.fragment_sampler_index);
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
  if (resource.vertex_global_uniform_block != nullptr) {
    resource.vertex_global_uniform_block->Release();
  }
  if (resource.fragment_global_uniform_block != nullptr) {
    resource.fragment_global_uniform_block->Release();
  }
}

} // namespace xEngine

#endif // X_D3D11
