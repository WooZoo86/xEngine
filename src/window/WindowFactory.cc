#include "WindowFactory.h"

#if X_WINDOWS || X_MACOS || X_LINUX
# include "window/glfw/GLFWWindow.h"
#endif // X_WINDOWS || X_MACOS || X_LINUX

namespace xEngine {

void WindowFactory::Create(WindowResource &resource, DataPtr /*data*/) {
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();
#if X_WINDOWS || X_MACOS || X_LINUX
  resource.window_.reset(new GLFWWindow);
#endif
  resource.window()->Create(resource.config());
  resource.Complete();
}

void WindowFactory::Destroy(WindowResource &resource) {
  x_assert(resource.status() == ResourceStatus::kCompleted);
  resource.window()->Destroy();
}

}
