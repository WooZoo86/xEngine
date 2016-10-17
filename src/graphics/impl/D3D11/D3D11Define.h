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

#define x_d3d11_assert(call) x_assert(SUCCEEDED(call))

#define x_d3d11_assert_msg(call, msg) x_assert_msg(SUCCEEDED(call), msg)

namespace xEngine {

struct D3DRendererCache {
  ID3D11RenderTargetView *render_target_view{nullptr};

  ID3D11DepthStencilView *depth_stencil_view{nullptr};

  ID3D11BlendState *blend_state{nullptr};

  ID3D11DepthStencilState *depth_stencil_state{nullptr};

  ID3D11RasterizerState *rasterizer_state{nullptr};

  D3D11_VIEWPORT viewport;

  D3D11_RECT scissor;

  Color blend_color;

  ID3D11InputLayout *input_layout{nullptr};

  ID3D11Buffer *vertex_buffer{nullptr};

  ID3D11Buffer *index_buffer{nullptr};

  ID3D11VertexShader *vertex_shader{nullptr};

  ID3D11PixelShader *fragment_shader{nullptr};
};

static DXGI_FORMAT SwapChainFormatFromPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8:
      return DXGI_FORMAT_R8G8B8A8_UNORM;
    default:
      x_error("unknown PixelFormat for swap chain\n");
      return DXGI_FORMAT_UNKNOWN;
  }
}

static DXGI_FORMAT RenderTargetFormatFromPixelFormat(PixelFormat format) {
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

static D3D11_USAGE D3D11UsageForBufferUsage(BufferUsage usage) {
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

static UINT D3D11CPUAccessFlagForBufferUsage(BufferUsage usage) {
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

static DXGI_FORMAT TextureFormatForPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case PixelFormat::RGBA4: return DXGI_FORMAT_B4G4R4A4_UNORM;
    case PixelFormat::R5G6B5: return DXGI_FORMAT_B5G6R5_UNORM;
    case PixelFormat::R5G5B5A1: return DXGI_FORMAT_B5G5R5A1_UNORM;
    case PixelFormat::RGBA32F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case PixelFormat::RGBA16F: return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case PixelFormat::L8: return DXGI_FORMAT_R8_UNORM;
    case PixelFormat::DXT1: return DXGI_FORMAT_BC1_UNORM;
    case PixelFormat::DXT3: return DXGI_FORMAT_BC2_UNORM;
    case PixelFormat::DXT5: return DXGI_FORMAT_BC3_UNORM;
    case PixelFormat::D16: return DXGI_FORMAT_D16_UNORM;
    case PixelFormat::D32: return DXGI_FORMAT_D32_FLOAT;
    case PixelFormat::D24S8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
    default: x_error("unknown PixelFormat\n");
      return DXGI_FORMAT_UNKNOWN;
  }
}

static D3D11_FILTER SamplerFilterForTextureFilterMode(TextureFilterMode min, TextureFilterMode mag) {
  if (TextureFilterMode::kNearest == mag) {
    switch (min) {
      case TextureFilterMode::kNearest:
      case TextureFilterMode::kNearestMipmapNearest:
        return D3D11_FILTER_MIN_MAG_MIP_POINT;
      case TextureFilterMode::kLinear:
      case TextureFilterMode::kLinearMipmapNearest:
        return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
      case TextureFilterMode::kNearestMipmapLinear:
        return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
      case TextureFilterMode::kLinearMipmapLinear:
        return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
      default:
        break;
    }
  } else if (TextureFilterMode::kLinear == mag) {
    switch (mag) {
      case TextureFilterMode::kNearest:
      case TextureFilterMode::kNearestMipmapNearest:
        return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
      case TextureFilterMode::kLinear:
      case TextureFilterMode::kLinearMipmapNearest:
        return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
      case TextureFilterMode::kNearestMipmapLinear:
        return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
      case TextureFilterMode::kLinearMipmapLinear:
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      default:
        break;
    }
  }
  x_error("SamplerFilterForTextureFilterMode invalid filter combination!\n");
  return D3D11_FILTER_MIN_MAG_MIP_POINT;
}

static D3D11_TEXTURE_ADDRESS_MODE TextureAddressModeForTextureWrapMode(TextureWrapMode mode) {
  switch (mode) {
    case TextureWrapMode::kClampToEdge:
      return D3D11_TEXTURE_ADDRESS_CLAMP;
    case TextureWrapMode::kRepeat:
      return D3D11_TEXTURE_ADDRESS_WRAP;
    case TextureWrapMode::kMirroredRepeat:
      return D3D11_TEXTURE_ADDRESS_MIRROR;
    default:
      return D3D11_TEXTURE_ADDRESS_CLAMP;
  }
}

static uint32 RowPitchForPixelFormat(PixelFormat format, uint32 width) {
  switch (format) {
    case PixelFormat::DXT1:
    case PixelFormat::ETC2_RGB8:
    case PixelFormat::ETC2_SRGB8:
      return max(8, 2 * width + 6);
    case PixelFormat::DXT3:
    case PixelFormat::DXT5:
      return max(16, 4 * width + 12);
    case PixelFormat::PVRTC4_RGB:
    case PixelFormat::PVRTC4_RGBA:
    case PixelFormat::PVRTC2_RGB:
    case PixelFormat::PVRTC2_RGBA:
      return max(16, 2 * width);
    default:
      return width * SizeOfPixelFormat(format);
  }
}

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_D3D11DEFINE_H
