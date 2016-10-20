#ifndef XENGINE_GRAPHICS_CONFIG_UNIFORMBUFFERCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_UNIFORMBUFFERCONFIG_H

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace xEngine {

struct UniformElement {

  eastl::string name;

  UniformFormat format{UniformFormat::kInvalid};

  size_t offset{0};

};

struct UniformBufferConfig {

  UniformBufferConfig &AddElement(const eastl::string &name, UniformFormat format) {
    UniformElement element;
    element.name = name;
    element.format = format;
    elements.push_back(element);
    return *this;
  }

  ResourceIdentity identity{ResourceIdentity::Shared(UniformBufferSignature)};

  eastl::vector<UniformElement> elements;

  size_t size{0};

};

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_UNIFORMBUFFERCONFIG_H