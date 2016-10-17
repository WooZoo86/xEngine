#if X_D3D11

#include "D3D11ShaderFactory.h"

namespace xEngine {

void D3D11ShaderFactory::Create(D3D11Shader &resource) {
	x_assert(resource.status() == ResourceStatus::kPending);
	resource.Loading();

	resource.Complete();
}

void D3D11ShaderFactory::Destroy(D3D11Shader &resource) {
	x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
}

} // namespace xEngine

#endif // X_D3D11
