#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_FACTORY_D3D11VERTEXDATAFACTORY_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_FACTORY_D3D11VERTEXDATAFACTORY_H

#if X_D3D11

#include "graphics/impl/D3D11/resource/D3D11GraphicsResource.h"

#include "resource/ResourceFactory.h"

namespace xEngine {

class D3D11VertexDataFactory : public ResourceFactory<D3D11VertexData> {
 public:
	explicit D3D11VertexDataFactory(ID3D11Device *device) : device_(device) {}

	virtual void Create(D3D11VertexData &resource) override;

	virtual void Destroy(D3D11VertexData &resource) override;

 private:
	ID3D11Device *device_{nullptr};
};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_FACTORY_D3D11VERTEXDATAFACTORY_H
