#include "Window.h"

#if X_USE_GLFW
# include "window/glfw/GLFWWindow.h"
#elif X_WINDOWS
# include "window/win32/Win32Window.h"
#endif

namespace xEngine {

IncreaseResourcePoolIDCounter();
constexpr ResourcePoolID g_window_pool_resource_id = GetResourcePoolID();

void Window::Initialize(uint16 pool_size) {
  x_assert(!Available());
#if X_USE_GLFW
  GLFWWindow::Initialize();
#elif X_WINDOWS
  Win32Window::Initialize();
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
#if X_USE_GLFW
  GLFWWindow::Finalize();
#elif X_WINDOWS
  Win32Window::Finalize();
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

void Window::MakeCurrent(ResourceID id) {
  x_assert(Available());
  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    resource.window->MakeCurrent();
  }
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
#if X_USE_GLFW
  GLFWWindow::PollEvent();
#elif X_WINDOWS
  Win32Window::PollEvent();
#endif
}

void Window::PresentAllWindow() {
  x_assert(Available());
  for (auto &id : resource_id_cache_) {
    auto &resource = pool_.Find(id);
    if (resource.status() != ResourceStatus::kInvalid) {
      resource.window->Present();
    }
  }
}

}