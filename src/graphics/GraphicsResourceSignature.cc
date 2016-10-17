#include "GraphicsResourceSignature.h"

namespace xEngine {

IncreaseResourceSignatureCounter();
constexpr ResourceSignature TextureSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
constexpr ResourceSignature ShaderSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
constexpr ResourceSignature MeshSignature = GetResourceSignature();

IncreaseResourceSignatureCounter();
constexpr ResourceSignature PipelineSignature = GetResourceSignature();

} // namespace xEngine