#ifndef XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H

#include "core/Data.h"

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include <EASTL/string.h>

namespace xEngine {

struct ShaderConfig {

  static ShaderConfig FromBuffer(const uchar *vertex, size_t vertex_length, const uchar *fragment, size_t fragment_length);

  static ShaderConfig FromData(const DataPtr &vertex, const DataPtr &fragment);

  ResourceIdentity identity{ResourceIdentity::Shared(ShaderSignature)};

  DataPtr vertex;

  DataPtr fragment;

};

inline ShaderConfig ShaderConfig::FromBuffer(const uchar *vertex, size_t vertex_length, const uchar *fragment, size_t fragment_length) {
  ShaderConfig config;
  config.vertex = Data::Create(vertex, vertex_length);
  config.fragment = Data::Create(fragment, fragment_length);
  return config;
}

inline ShaderConfig ShaderConfig::FromData(const DataPtr &vertex, const DataPtr &fragment) {
  ShaderConfig config;
  config.vertex = Data::Create(vertex->buffer(), vertex->size());
  config.fragment = Data::Create(fragment->buffer(), fragment->size());
  return config;
}

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_SHADERCONFIG_H