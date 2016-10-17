#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H

#if X_D3D11

#include "D3D11GraphicsResource.h"
#include "graphics/GraphicsResourceManagerInterface.h"

#include "factory/D3D11ShaderFactory.h"
#include "factory/D3D11VertexDataFactory.h"
#include "factory/D3D11IndexDataFactory.h"
#include "factory/D3D11TextureFactory.h"

#include "resource/ResourcePool.h"

namespace xEngine {

typedef ResourcePool<D3D11Shader, ShaderConfig> D3D11ShaderPool;
typedef ResourcePool<D3D11VertexData, VertexDataConfig> D3D11VertexDataPool;
typedef ResourcePool<D3D11IndexData, IndexDataConfig> D3D11IndexDataPool;
typedef ResourcePool<D3D11Texture, TextureConfig> D3D11TexturePool;

class D3D11GraphicsResourceManager: public GraphicsResourceManagerInterface {
 public:
	explicit D3D11GraphicsResourceManager(ID3D11Device *device) :
		shader_factory_(device),
		vertex_data_factory_(device),
		index_data_factory_(device),
		texture_factory_(device) {}

	virtual void Initialize(const GraphicsConfig &config) override;

	virtual void Finalize() override;

	virtual ResourceID Create(const ShaderConfig &config) override;

	virtual ResourceID Create(const VertexDataConfig &config) override;

	virtual ResourceID Create(const IndexDataConfig &config) override;

	virtual ResourceID Create(const TextureConfig &config) override;

	virtual void Destroy(ResourceID id) override;

 private:
	D3D11ShaderPool shader_pool_;
	D3D11VertexDataPool vertex_data_pool_;
	D3D11IndexDataPool index_data_pool_;
	D3D11TexturePool texture_pool_;

	D3D11ShaderFactory shader_factory_;
	D3D11VertexDataFactory vertex_data_factory_;
	D3D11IndexDataFactory index_data_factory_;
	D3D11TextureFactory texture_factory_;

	friend class D3D11Renderer;
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCEMANAGER_H
