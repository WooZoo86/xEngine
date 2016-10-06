#ifndef XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H

#include "graphics/GraphicsDefine.h"

#include "core/Data.h"
#include "resource/ResourceIdentity.h"

namespace xEngine {

IncreaseResourceSignatureCounter();
static const ResourceSignature ShaderSignature = GetResourceSignature();

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

}

#endif // XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H