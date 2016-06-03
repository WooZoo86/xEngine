#ifndef XENGINE_WINDOW_WINDOWINTERFACE_H
#define XENGINE_WINDOW_WINDOWINTERFACE_H

#include "WindowConfig.h"

#include <EASTL/string.h>

namespace xEngine {

class WindowInterface {
 public:
  virtual ~WindowInterface() { }

  virtual void Initialize(const WindowConfig &config) = 0;

  virtual void Finalize() = 0;

  virtual bool Available() = 0;

  virtual bool ShouldClose() = 0;

  virtual void PollEvent() = 0;

  virtual void Present() = 0;

  virtual void SetTitle(const eastl::string &name) = 0;

  virtual const WindowConfig &GetConfig() const = 0;
};

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOWINTERFACE_H