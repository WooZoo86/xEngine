#if X_D3D11

#include "D3D11PipelineFactory.h"
#include "graphics/impl/D3D11/D3D11Define.h"

#include "core/Log.h"

namespace xEngine {

void D3D11PipelineFactory::Create(D3D11Pipeline &resource) {
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();

  resource.Complete();
}

void D3D11PipelineFactory::Destroy(D3D11Pipeline &resource) {
  x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
}

} // namespace xEngine

#endif // X_D3D11