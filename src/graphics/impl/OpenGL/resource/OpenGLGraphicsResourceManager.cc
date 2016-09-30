#if X_OPENGL

#include "OpenGLGraphicsResourceManager.h"

namespace xEngine {

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_shader_resource_pool_id = GetResourcePoolID();

void OpenGLGraphicsResourceManager::Initialize(const GraphicsConfig &config) {
  shader_pool_.Initialize(config.shader_pool_size, g_opengl_shader_resource_pool_id);
}

void OpenGLGraphicsResourceManager::Finalize() {
  for (auto &id : resource_id_cache_) {
    switch (GetResourcePoolIDOfResourceID(id)) {
      case g_opengl_shader_resource_pool_id: {
        auto &resource = shader_pool_.Find(id);
        if (resource.status() != ResourceStatus::kInvalid) {
          shader_factory_.Destroy(resource);
        }
        shader_pool_.Destroy(id);
        break;
      }
    }
  }
  shader_pool_.Finalize();
  RemoveAll();
}

ResourceID OpenGLGraphicsResourceManager::Create(const ShaderConfig &config) {
  auto id = shader_pool_.Create(config);
  Add(config.identity, id);
  auto &resource = shader_pool_.Find(id);
  shader_factory_.Create(resource);
  return id;
}

void OpenGLGraphicsResourceManager::Destroy(ResourceID id) {
  switch (GetResourcePoolIDOfResourceID(id)) {
    case g_opengl_shader_resource_pool_id: {
      auto &resource = shader_pool_.Find(id);
      if (resource.status() != ResourceStatus::kInvalid) {
        shader_factory_.Destroy(resource);
      }
      shader_pool_.Destroy(id);
      break;
    }
  }
  Remove(id);
}

}

#endif // X_OPENGL
