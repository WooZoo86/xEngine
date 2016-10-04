#include "WindowFactory.h"

#if X_USE_GLFW
# include "window/glfw/GLFWWindow.h"
#elif X_WINDOWS
# include "window/win32/Win32Window.h"
#endif

namespace xEngine {

void WindowFactory::Create(WindowResource &resource) {
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();
#if X_USE_GLFW
  resource.window_.reset(new GLFWWindow);
#elif X_WINDOWS
  resource.window_.reset(new Win32Window);
#endif
  if (resource.window() != nullptr) {
    resource.window()->Create(resource.config());
  }
  resource.window() == nullptr ? resource.Failed() : resource.Complete();
}

void WindowFactory::Destroy(WindowResource &resource) {
  x_assert(resource.status() == ResourceStatus::kCompleted || resource.status() == ResourceStatus::kFailed);
  resource.window()->Destroy();
}

}
