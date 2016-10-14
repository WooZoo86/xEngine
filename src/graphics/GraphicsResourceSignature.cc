#include "GraphicsResourceSignature.h"

namespace xEngine {

IncreaseResourceSignatureCounter();
const ResourceSignature TextureSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
const ResourceSignature ShaderSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
const ResourceSignature MeshSignature = GetResourceSignature();

} // namespace xEngine