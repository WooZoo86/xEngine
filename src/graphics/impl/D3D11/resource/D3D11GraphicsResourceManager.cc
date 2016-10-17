#if X_D3D11

#include "D3D11GraphicsResourceManager.h"

namespace xEngine {

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_shader_resource_pool_id = GetResourcePoolID();

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_vertex_data_resource_pool_id = GetResourcePoolID();

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_index_data_resource_pool_id = GetResourcePoolID();

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_opengl_texture_resource_pool_id = GetResourcePoolID();

void D3D11GraphicsResourceManager::Initialize(const GraphicsConfig &config) {
	shader_pool_.Initialize(config.shader_pool_size, g_opengl_shader_resource_pool_id);
	vertex_data_pool_.Initialize(config.vertex_data_pool_size, g_opengl_vertex_data_resource_pool_id);
	index_data_pool_.Initialize(config.index_data_pool_size, g_opengl_index_data_resource_pool_id);
	texture_pool_.Initialize(config.texture_pool_size, g_opengl_texture_resource_pool_id);
}

void D3D11GraphicsResourceManager::Finalize() {
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
			case g_opengl_vertex_data_resource_pool_id: {
				auto &resource = vertex_data_pool_.Find(id);
				if (resource.status() != ResourceStatus::kInvalid) {
					vertex_data_factory_.Destroy(resource);
				}
				vertex_data_pool_.Destroy(id);
				break;
			}
			case g_opengl_index_data_resource_pool_id: {
				auto &resource = index_data_pool_.Find(id);
				if (resource.status() != ResourceStatus::kInvalid) {
					index_data_factory_.Destroy(resource);
				}
				index_data_pool_.Destroy(id);
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
			default: break;
		}
	}
	shader_pool_.Finalize();
	vertex_data_pool_.Finalize();
	index_data_pool_.Finalize();
	texture_pool_.Finalize();
	RemoveAll();
}

ResourceID D3D11GraphicsResourceManager::Create(const ShaderConfig &config) {
	auto id = shader_pool_.Create(config);
	Add(config.identity, id);
	auto &resource = shader_pool_.Find(id);
	shader_factory_.Create(resource);
	return id;
}

ResourceID D3D11GraphicsResourceManager::Create(const VertexDataConfig &config) {
	auto id = vertex_data_pool_.Create(config);
	Add(config.identity, id);
	auto &resource = vertex_data_pool_.Find(id);
	vertex_data_factory_.Create(resource);
	return id;
}

ResourceID D3D11GraphicsResourceManager::Create(const IndexDataConfig &config) {
	auto id = index_data_pool_.Create(config);
	Add(config.identity, id);
	auto &resource = index_data_pool_.Find(id);
	index_data_factory_.Create(resource);
	return id;
}

ResourceID D3D11GraphicsResourceManager::Create(const TextureConfig &config) {
	auto id = texture_pool_.Create(config);
	Add(config.identity, id);
	auto &resource = texture_pool_.Find(id);
	texture_factory_.Create(resource);
	return id;
}

void D3D11GraphicsResourceManager::Destroy(ResourceID id) {
	switch (GetResourcePoolIDOfResourceID(id)) {
		case g_opengl_shader_resource_pool_id: {
			auto &resource = shader_pool_.Find(id);
			if (resource.status() != ResourceStatus::kInvalid) {
				shader_factory_.Destroy(resource);
			}
			shader_pool_.Destroy(id);
			break;
		}
		case g_opengl_vertex_data_resource_pool_id: {
			auto &resource = vertex_data_pool_.Find(id);
			if (resource.status() != ResourceStatus::kInvalid) {
				vertex_data_factory_.Destroy(resource);
			}
			vertex_data_pool_.Destroy(id);
			break;
		}
		case g_opengl_index_data_resource_pool_id: {
			auto &resource = index_data_pool_.Find(id);
			if (resource.status() != ResourceStatus::kInvalid) {
				index_data_factory_.Destroy(resource);
			}
			index_data_pool_.Destroy(id);
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
		default: break;
	}
	Remove(id);
}

}

#endif