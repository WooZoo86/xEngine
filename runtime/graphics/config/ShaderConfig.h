#ifndef XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include "core/Data.h"

namespace xEngine {

struct ShaderConfig {

  static ShaderConfig FromData(DataPtr vertex, DataPtr fragment);

  ResourceIdentity identity{ResourceIdentity::Shared(ShaderSignature)};

  DataPtr vertex;

  DataPtr fragment;

};

inline ShaderConfig ShaderConfig::FromData(DataPtr vertex, DataPtr fragment) {
  ShaderConfig config;
  config.vertex = vertex;
  config.fragment = fragment;
  return config;
}

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H