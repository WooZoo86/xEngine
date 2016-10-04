#ifndef XENGINE_GRAPHICS_CONFIG_GRAPHICSCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_GRAPHICSCONFIG_H

#include "graphics/GraphicsDefine.h"

#include <resource/Resource.h>

namespace xEngine {

struct GraphicsConfig {

  static GraphicsConfig ForWindow(ResourceID id);

  ResourceID window{kInvalidResourceID};

#if X_WINDOWS
  GraphicsType type{GraphicsType::kD3D11};
#else
  GraphicsType type{GraphicsType::kOpenGL3};
#endif

  uint16 shader_pool_size{256};

};

inline GraphicsConfig GraphicsConfig::ForWindow(ResourceID id) {
  GraphicsConfig config;
  config.window = id;
  return config;
}

}

#endif // XENGINE_GRAPHICS_CONFIG_GRAPHICSCONFIG_H