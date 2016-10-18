#if X_D3D11

#include "D3D11ShaderFactory.h"

#include "graphics/impl/D3D11/D3D11Define.h"

#include "core/Log.h"

#include <D3Dcompiler.h>

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
  }

  if (fragment_blob != nullptr) {
    x_d3d11_assert_msg(device_->CreatePixelShader(
      fragment_blob->GetBufferPointer(),
      fragment_blob->GetBufferSize(),
      nullptr,
      &fragment_shader
    ), "create fragment shader failed\n");
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
}

} // namespace xEngine

#endif // X_D3D11
