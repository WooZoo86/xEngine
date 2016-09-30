#ifndef XENGINE_GRAPHICS_CONFIG_GRAPHICSCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_GRAPHICSCONFIG_H

#include "graphics/GraphicsDefine.h"

#include <resource/Resource.h>

namespace xEngine {

struct GraphicsConfig {

  ResourceID window{kInvalidResourceID};

  GraphicsType type{GraphicsType::kOpenGL3};

  uint16 shader_pool_size{256};

};

}

#endif // XENGINE_GRAPHICS_CONFIG_GRAPHICSCONFIG_H