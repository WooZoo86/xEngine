#ifndef XENGINE_GRAPHICS_GRAPHICSINTERFACE_H
#define XENGINE_GRAPHICS_GRAPHICSINTERFACE_H

#include "graphics/config/GraphicsConfig.h"
#include "graphics/state/ClearState.h"

#include <resource/Resource.h>

namespace xEngine {

class RendererInterface {
 public:
  virtual ~RendererInterface() {}

  virtual void Initialize(const GraphicsConfig &config) = 0;

  virtual void Finalize() = 0;

  virtual void ApplyTarget(ResourceID id = kInvalidResourceID) = 0;

  virtual void ApplyClearState(const ClearState &state) = 0;

  virtual void Reset() = 0;
};

}

#endif // XENGINE_GRAPHICS_GRAPHICSINTERFACE_H