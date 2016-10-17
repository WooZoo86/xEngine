#ifndef XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H
#define XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H

#if X_D3D11

#include "graphics/GraphicsDefine.h"
#include "graphics/config/GraphicsConfig.h"
#include "graphics/state/BlendState.h"
#include "graphics/state/DepthStencilState.h"
#include "graphics/state/RasterizerState.h"

#include "graphics/impl/D3D11/resource/D3D11GraphicsResource.h"

#include "window/WindowDefine.h"

#include <D3D11.h>

#include <EASTL/hash_map.h>
#include <EASTL/string.h>



#define x_d3d11_assert(call, msg) x_assert_msg(SUCCEEDED(call), msg)

namespace xEngine {

inline DXGI_FORMAT SwapChainFormatFromPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8:
      return DXGI_FORMAT_R8G8B8A8_UNORM;
    default:
      x_error("unknown PixelFormat for swap chain\n");
      return DXGI_FORMAT_UNKNOWN;
  }
}

inline DXGI_FORMAT RenderTargetFormatFromPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8:
      return DXGI_FORMAT_R8G8B8A8_UNORM;
    case PixelFormat::RGBA32F:
      return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case PixelFormat::RGBA16F:
      return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case PixelFormat::D16:
      return DXGI_FORMAT_D16_UNORM;
    case PixelFormat::D24S8:
      return DXGI_FORMAT_D24_UNORM_S8_UINT;
    default:
      x_error("unknown PixelFormat for render target\n");
      return DXGI_FORMAT_UNKNOWN;
  }
}

inline D3D11_USAGE D3D11UsageForBufferUsage(BufferUsage usage) {
	switch (usage) {
		case BufferUsage::kImmutable:
			return D3D11_USAGE_IMMUTABLE;
		case BufferUsage::kDynamic:
			return D3D11_USAGE_DYNAMIC;
		case BufferUsage::kStream:
			return D3D11_USAGE_DYNAMIC;
		default:
			x_error("unknown BufferUsage\n");
			return D3D11_USAGE_IMMUTABLE;
	}
}

inline UINT D3D11CPUAccessFlagForBufferUsage(BufferUsage usage) {
	switch (usage) {
	case BufferUsage::kImmutable:
		return 0;
	case BufferUsage::kDynamic:
		return D3D11_CPU_ACCESS_READ;
	case BufferUsage::kStream:
		return D3D11_CPU_ACCESS_READ;
	default:
		x_error("unknown BufferUsage\n");
		return 0;
	}
}

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H
