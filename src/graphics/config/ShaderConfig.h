#ifndef XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include "core/Data.h"

namespace xEngine {

struct ShaderConfig {

  static ShaderConfig FromData(const char *vertex, const char *fragment);

  ResourceIdentity identity{ResourceIdentity::Shared(ShaderSignature)};

  const char *vertex;

  const char *fragment;

};

inline ShaderConfig ShaderConfig::FromData(const char *vertex, const char *fragment) {
  ShaderConfig config;
  config.vertex = vertex;
  config.fragment = fragment;
  return config;
}

}

#endif // XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H