#ifndef XENGINE_GRAPHICS_GRAPHICSRESOURCEMANAGERINTERFACE_H
#define XENGINE_GRAPHICS_GRAPHICSRESOURCEMANAGERINTERFACE_H

#include "config/GraphicsConfig.h"
#include "config/ShaderConfig.h"
#include "config/VertexDataConfig.h"
#include "config/IndexDataConfig.h"
#include "config/TextureConfig.h"

#include "resource/Resource.h"
#include "resource/ResourceManager.h"

namespace xEngine {

class GraphicsResourceManagerInterface: public ResourceManager {
 public:
  virtual ~GraphicsResourceManagerInterface() {}

  virtual void Initialize(const GraphicsConfig &config) = 0;

  virtual void Finalize() = 0;

  virtual ResourceID Create(const ShaderConfig &config) = 0;

  virtual ResourceID Create(const VertexDataConfig &config) = 0;

  virtual ResourceID Create(const IndexDataConfig &config) = 0;

  virtual ResourceID Create(const TextureConfig &config) = 0;

  virtual void Destroy(ResourceID id) = 0;
};

}

#endif // XENGINE_GRAPHICS_GRAPHICSRESOURCEMANAGERINTERFACE_H