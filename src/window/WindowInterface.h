#ifndef XENGINE_WINDOW_WINDOWINTERFACE_H
#define XENGINE_WINDOW_WINDOWINTERFACE_H

#include "WindowConfig.h"

#include "graphics/Graphics.h"

#include <EASTL/string.h>
#include <EASTL/unique_ptr.h>

namespace xEngine {

class WindowInterface {
 public:
  virtual ~WindowInterface() {}

  virtual void Create(const WindowConfig &config) = 0;

  virtual void Destroy() = 0;

  virtual bool Available() const = 0;

  virtual void MakeCurrent() = 0;

  virtual bool ShouldClose() = 0;

  virtual void Present() = 0;

  virtual void SetTitle(const eastl::string &name) = 0;

  virtual void *GetNativeHandle() = 0;

  const WindowConfig &config() const { return config_; }

  const eastl::unique_ptr<Graphics> &graphics() const { return graphics_; }

 protected:
  WindowConfig config_;
  eastl::unique_ptr<Graphics> graphics_{new Graphics};
};

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOWINTERFACE_H