#if X_OPENGL

#include "OpenGLGraphicsResourceManager.h"

namespace xEngine {

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_shader_resource_pool_id = GetResourcePoolID();

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_texture_resource_pool_id = GetResourcePoolID();

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_mesh_resource_pool_id = GetResourcePoolID();

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_pipeline_resource_pool_id = GetResourcePoolID();

void OpenGLGraphicsResourceManager::Initialize(const GraphicsConfig &config) {
  shader_pool_.Initialize(config.shader_pool_size, g_opengl_shader_resource_pool_id);
  texture_pool_.Initialize(config.texture_pool_size, g_opengl_texture_resource_pool_id);
  mesh_pool_.Initialize(config.mesh_pool_size, g_opengl_mesh_resource_pool_id);
  pipeline_pool_.Initialize(config.pipeline_pool_size, g_opengl_pipeline_resource_pool_id);
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
      case g_opengl_texture_resource_pool_id: {
        auto &resource = texture_pool_.Find(id);
        if (resource.status() != ResourceStatus::kInvalid) {
          texture_factory_.Destroy(resource);
        }
        texture_pool_.Destroy(id);
        break;
      }
      case g_opengl_mesh_resource_pool_id: {
        auto &resource = mesh_pool_.Find(id);
        if (resource.status() != ResourceStatus::kInvalid) {
          mesh_factory_.Destroy(resource);
        }
        mesh_pool_.Destroy(id);
        break;
      }
      case g_opengl_pipeline_resource_pool_id: {
        auto &resource = pipeline_pool_.Find(id);
        if (resource.status() != ResourceStatus::kInvalid) {
          pipeline_factory_.Destroy(resource);
        }
        pipeline_pool_.Destroy(id);
        break;
      }
    }
  }
  shader_pool_.Finalize();
  texture_pool_.Finalize();
  mesh_pool_.Finalize();
  pipeline_pool_.Finalize();
  RemoveAll();
}

ResourceID OpenGLGraphicsResourceManager::Create(const ShaderConfig &config) {
  auto id = shader_pool_.Create(config);
  Add(config.identity, id);
  auto &resource = shader_pool_.Find(id);
  shader_factory_.Create(resource);
  return id;
}

ResourceID OpenGLGraphicsResourceManager::Create(const TextureConfig &config) {
  auto id = texture_pool_.Create(config);
  Add(config.identity, id);
  auto &resource = texture_pool_.Find(id);
  texture_factory_.Create(resource);
  return id;
}

ResourceID OpenGLGraphicsResourceManager::Create(const MeshConfig &config) {
  auto id = mesh_pool_.Create(config);
  Add(config.identity, id);
  auto &resource = mesh_pool_.Find(id);
  mesh_factory_.Create(resource);
  return id;
}

ResourceID OpenGLGraphicsResourceManager::Create(const PipelineConfig &config) {
  auto id = pipeline_pool_.Create(config);
  Add(config.identity, id);
  auto &resource = pipeline_pool_.Find(id);
  pipeline_factory_.Create(resource);
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
    case g_opengl_texture_resource_pool_id: {
      auto &resource = texture_pool_.Find(id);
      if (resource.status() != ResourceStatus::kInvalid) {
        texture_factory_.Destroy(resource);
      }
      texture_pool_.Destroy(id);
      break;
    }
    case g_opengl_mesh_resource_pool_id: {
      auto &resource = mesh_pool_.Find(id);
      if (resource.status() != ResourceStatus::kInvalid) {
        mesh_factory_.Destroy(resource);
      }
      mesh_pool_.Destroy(id);
      break;
    }
    case g_opengl_pipeline_resource_pool_id: {
      auto &resource = pipeline_pool_.Find(id);
      if (resource.status() != ResourceStatus::kInvalid) {
        pipeline_factory_.Destroy(resource);
      }
      pipeline_pool_.Destroy(id);
      break;
    }
  }
  Remove(id);
}

}

#endif // X_OPENGL
