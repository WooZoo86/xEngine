#if X_D3D11

#include "D3D11TextureFactory.h"

namespace xEngine {

void D3D11TextureFactory::Create(D3D11Texture &resource) {
	x_assert(resource.status() == ResourceStatus::kPending);
	resource.Loading();

	resource.Complete();
}

void D3D11TextureFactory::Destroy(D3D11Texture &resource) {
	x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
}

} // namespace xEngine

#endif // X_D3D11
