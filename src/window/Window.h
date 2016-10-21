#ifndef XENGINE_WINDOW_WINDOW_H
#define XENGINE_WINDOW_WINDOW_H

#include "WindowInterface.h"
#include "WindowFactory.h"

#include "resource/ResourceManager.h"
#include "resource/ResourcePool.h"

namespace xEngine {

typedef ResourcePool<WindowResource, WindowConfig> WindowPool;

class Window: public ResourceManager {
 public:
  static Window &GetInstance() {
    static Window instance;
    return instance;
  }

 public:
  void Initialize(uint16 pool_size = 5);

  void Finalize();

  bool Available() const;

  ResourceID Create(const WindowConfig &config);

  void Destroy(ResourceID id);

  const eastl::unique_ptr<WindowInterface> &Get(ResourceID id);

  bool ShouldClose(ResourceID id);

  const eastl::unique_ptr<Graphics> &GetGraphics(ResourceID id);

  bool IsAllClosed();

  void PollEvent();

 private:
  bool available_{false};
  WindowFactory factory_;
  WindowPool pool_;
};

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOW_H