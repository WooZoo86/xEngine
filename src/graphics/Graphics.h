#ifndef XENGINE_GRAPHICS_GRAPHICS_H
#define XENGINE_GRAPHICS_GRAPHICS_H

#include "GraphicsResourceManagerInterface.h"
#include "RendererInterface.h"
#include "config/GraphicsConfig.h"

#include <EASTL/unique_ptr.h>

namespace xEngine {

class Graphics {
 public:
  static Graphics &GetInstance() {
    static Graphics instance;
    return instance;
  }

 public:
  void Initialize(const GraphicsConfig &config);

  void Finalize();

  bool Available();

  const eastl::unique_ptr<RendererInterface> &renderer() const { return renderer_; }

  const eastl::unique_ptr<GraphicsResourceManagerInterface> &resource_manager() const { return resource_manager_; }

 private:
  GraphicsConfig config_;
  eastl::unique_ptr<RendererInterface> renderer_{nullptr};
  eastl::unique_ptr<GraphicsResourceManagerInterface> resource_manager_{nullptr};
};

}

#endif // XENGINE_GRAPHICS_GRAPHICS_H