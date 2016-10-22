#include "Window.h"

#if X_WINDOWS
# include "window/win32/Win32Window.h"
#elif X_MACOS
# include "window/macOS/MacOSWindow.h"
#endif

namespace xEngine {

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_window_pool_resource_id = GetResourcePoolID();

void Window::Initialize(uint16 pool_size) {
  x_assert(!Available());
#if X_WINDOWS
  Win32Window::Initialize();
#elif X_MACOS
  MacOSWindow::Initialize();
#endif
  pool_.Initialize(pool_size, g_window_pool_resource_id);
  available_ = true;
}

void Window::Finalize() {
  available_ = false;
  for (auto &id : resource_id_cache_) {
    auto &resource = pool_.Find(id);
    if (resource.status() != ResourceStatus::kInvalid) {
      factory_.Destroy(resource);
    }
    pool_.Destroy(id);
  }
  pool_.Finalize();
  RemoveAll();
#if X_WINDOWS
  Win32Window::Finalize();
#elif X_MACOS
  MacOSWindow::Finalize();
#endif
}

bool Window::Available() const {
  return available_;
}

ResourceID Window::Create(const WindowConfig &config) {
  x_assert(Available());
  auto id = pool_.Create(config);
  Add(config.identity, id);
  auto &resource = pool_.Find(id);
  factory_.Create(resource);
  return id;
}

void Window::Destroy(ResourceID id) {
  x_assert(Available());
  x_assert(Contains(id));
  Remove(id);
  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    factory_.Destroy(resource);
  }
  pool_.Destroy(id);
}

const eastl::unique_ptr<WindowInterface> &Window::Get(ResourceID id) {
  x_assert(Available());

  static const eastl::unique_ptr<WindowInterface> invalid = nullptr;

  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    return resource.window;
  }
  return invalid;
}

bool Window::ShouldClose(ResourceID id) {
  x_assert(Available());
  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    return resource.window->ShouldClose();
  }
  return false;
}

const eastl::unique_ptr<Graphics> &Window::GetGraphics(ResourceID id) {
  x_assert(Available());

  static const eastl::unique_ptr<Graphics> invalid = nullptr;

  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    return resource.window->graphics();
  }
  return invalid;
}

bool Window::IsAllClosed() {
  x_assert(Available());
  return resource_id_cache_.empty();
}

void Window::PollEvent() {
  x_assert(Available());
  for (auto &id : resource_id_cache_) {
    auto &resource = pool_.Find(id);
    if (resource.status() != ResourceStatus::kInvalid) {
      resource.window->ResetMouseStatus();
    }
  }
#if X_WINDOWS
  Win32Window::PollEvent();
#elif X_MACOS
  MacOSWindow::PollEvent();
#endif
}

}