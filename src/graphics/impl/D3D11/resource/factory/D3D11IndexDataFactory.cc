#if X_D3D11

#include "D3D11IndexDataFactory.h"

namespace xEngine {

void D3D11IndexDataFactory::Create(D3D11IndexData &resource) {
	x_assert(resource.status() == ResourceStatus::kPending);
	resource.Loading();

	ID3D11Buffer *buffer = nullptr;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = resource.config().data->size();
	desc.Usage = D3D11UsageForBufferUsage(resource.config().buffer_usage);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = D3D11CPUAccessFlagForBufferUsage(resource.config().buffer_usage);
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = resource.config().data->buffer();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	x_d3d11_assert(device_->CreateBuffer(&desc, &data, &buffer), "create vertex buffer fail\n");

	resource.buffer = buffer;

	resource.Complete();
}

void D3D11IndexDataFactory::Destroy(D3D11IndexData &resource) {
	x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
}

} // namespace xEngine

#endif // X_D3D11