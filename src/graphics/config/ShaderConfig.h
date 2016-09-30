#ifndef XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H

#include "graphics/GraphicsDefine.h"

#include <resource/ResourceIdentity.h>

namespace xEngine {

IncreaseResourceSignatureCounter();
static const ResourceSignature ShaderSignature = GetResourceSignature();

struct ShaderConfig {
  static ShaderConfig ForType(ShaderType type);

  ResourceIdentity identity{ResourceIdentity::Unique()};

  ShaderType type{ShaderType::kVertexShader};
};

inline ShaderConfig ShaderConfig::ForType(ShaderType type) {
  ShaderConfig config;
  config.type = type;
  return config;
}

}

#endif // XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H