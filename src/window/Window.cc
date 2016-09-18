#include "Window.h"

#if X_WINDOWS || X_MACOS || X_LINUX
# include "window/glfw/GLFWWindow.h"
#endif // X_WINDOWS || X_MACOS || X_LINUX

namespace xEngine {

IncreaseResourcePoolIDCounter();
static const ResourcePoolID g_window_pool_resource_id = GetResourcePoolID();

void Window::Initialize(uint16 pool_size) {
  x_assert(!Available());
#if X_WINDOWS || X_MACOS || X_LINUX
  GLFWWindow::Initialize();
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
#if X_WINDOWS || X_MACOS || X_LINUX
  GLFWWindow::Finalize();
#endif
}

bool Window::Available() {
  return available_;
}

ResourceID Window::Create(const WindowConfig &config) {
  x_assert(Available());
  auto id = pool_.Create(config);
  Add(config.identity, id);
  auto &resource = pool_.Find(id);
  factory_.Create(resource, nullptr);
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
    return resource.window();
  }
  return invalid;
}

void Window::MakeCurrent(ResourceID id) {
  x_assert(Available());
  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    resource.window()->MakeCurrent();
  }
}

bool Window::ShouldClose(ResourceID id) {
  x_assert(Available());
  auto &resource = pool_.Find(id);
  if (resource.status() != ResourceStatus::kInvalid) {
    return resource.window()->ShouldClose();
  }
  return false;
}

bool Window::IsAllClosed() {
  x_assert(Available());
  return resource_id_cache_.empty();
}

void Window::PollEvent() {
  x_assert(Available());
#if X_WINDOWS || X_MACOS || X_LINUX
  GLFWWindow::PollEvent();
#endif
}

void Window::PresentAllWindow() {
  x_assert(Available());
  for (auto &id : resource_id_cache_) {
    auto &resource = pool_.Find(id);
    if (resource.status() != ResourceStatus::kInvalid) {
      resource.window()->Present();
    }
  }
}

}