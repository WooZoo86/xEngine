#ifndef XENGINE_GRAPHICS_CONFIG_TEXTURECONFIG_H
#define XENGINE_GRAPHICS_CONFIG_TEXTURECONFIG_H

#include "graphics/GraphicsDefine.h"

#include "core/Data.h"
#include "resource/ResourceIdentity.h"
#include "window/WindowDefine.h"

namespace xEngine {

IncreaseResourceSignatureCounter();
static const ResourceSignature TextureSignature = GetResourceSignature();

struct TextureConfig {

  ResourceIdentity identity{ResourceIdentity::Shared(TextureSignature)};

  DataPtr data;

  TextureType type{TextureType::kTexture2d};

  PixelFormat color_format{PixelFormat::NONE};

  int32 width{0};

  int32 height{0};

  int32 mipmap_count{1};

  bool is_render_target{false};

  TextureWrapMode wrap_mod_s{TextureWrapMode::kRepeat};

  TextureWrapMode wrap_mod_t{TextureWrapMode::kRepeat};

  TextureWrapMode wrap_mod_r{TextureWrapMode::kRepeat};

  TextureFilterMode filter_mode_min{TextureFilterMode::kNearest};

  TextureFilterMode filter_mode_mag{TextureFilterMode::kNearest};

  size_t data_offset[static_cast<uint16>(GraphicsMaxDefine::kMaxCubeTextureFaceCount)][
      static_cast<uint16>(GraphicsMaxDefine::kMaxTextureMipMapCount)];

  size_t data_size[static_cast<uint16>(GraphicsMaxDefine::kMaxCubeTextureFaceCount)][
      static_cast<uint16>(GraphicsMaxDefine::kMaxTextureMipMapCount)];

  TextureConfig() {
    memset(data_offset, 0, sizeof(data_offset));
    memset(data_size, 0, sizeof(data_size));
  }
};

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_TEXTURECONFIG_H