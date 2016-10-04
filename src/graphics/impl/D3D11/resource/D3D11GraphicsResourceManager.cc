#if X_D3D11

#include "D3D11GraphicsResourceManager.h"

namespace xEngine {

void D3D11GraphicsResourceManager::Initialize(const GraphicsConfig &config) {}

void D3D11GraphicsResourceManager::Finalize() {}

ResourceID D3D11GraphicsResourceManager::Create(const ShaderConfig &config) {
  return kInvalidResourceID;
}

void D3D11GraphicsResourceManager::Destroy(ResourceID id) {}
}

#endif