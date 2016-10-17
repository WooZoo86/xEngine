#include "GraphicsResourceSignature.h"

namespace xEngine {

IncreaseResourceSignatureCounter();
static const ResourceSignature TextureSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
static const ResourceSignature ShaderSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
static const ResourceSignature MeshSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
static const ResourceSignature PipelineSignature = GetResourceSignature();

} // namespace xEngine