#ifndef XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include <EASTL/string.h>

namespace xEngine {

struct ShaderConfig {

  static ShaderConfig FromString(const eastl::string &vertex, const eastl::string &fragment);

  ResourceIdentity identity{ResourceIdentity::Shared(ShaderSignature)};

  eastl::string vertex;

  eastl::string fragment;

};

inline ShaderConfig ShaderConfig::FromString(const eastl::string &vertex, const eastl::string &fragment) {
  ShaderConfig config;
  config.vertex = vertex;
  config.fragment = fragment;
  return config;
}

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H