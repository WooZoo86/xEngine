#ifndef XENGINE_GRAPHICS_CONFIG_TEXTURECONFIG_H
#define XENGINE_GRAPHICS_CONFIG_TEXTURECONFIG_H

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include "core/Data.h"

#include "window/WindowDefine.h"

namespace xEngine {

struct TextureConfig {

  ResourceIdentity identity{ResourceIdentity::Shared(TextureSignature)};

  DataPtr data[static_cast<uint16>(GraphicsMaxDefine::kMaxCubeTextureFaceCount)][
      static_cast<uint16>(GraphicsMaxDefine::kMaxTextureMipMapCount)];

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
};

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_TEXTURECONFIG_H