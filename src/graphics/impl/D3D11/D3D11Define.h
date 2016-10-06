#ifndef XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H
#define XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H

#if X_D3D11

#include "core/Core.h"
#include "window/WindowDefine.h"

#include <d3d11.h>

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

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H
